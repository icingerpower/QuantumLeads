#include <QSettings>
#include <QDebug>

#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "Column.h"

#include "ColumnTreeItem.h"

#include "ColumnTree.h"

ColumnTree::ColumnTree(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_settingsKey = "ColumnTree";
    m_rootItem = new ColumnTreeItem;
    for (const auto &column : Column::ALL_COLUMNS)
    {
        new ColumnTreeItem{column.name(), column.id(), m_rootItem};
    }
    _loadFromSettings();
}

void ColumnTree::_clear()
{
    if (m_rootItem != nullptr)
    {
        beginRemoveRows(QModelIndex(), 0, m_rootItem->rowCount());
        delete m_rootItem;
        m_rootItem = new ColumnTreeItem;
        endRemoveRows();
    }
    else
    {
        m_rootItem = new ColumnTreeItem;
    }
}

void ColumnTree::_saveInSettings() const
{
    auto settings = WorkingDirectoryManager::instance()->settings();
    if (rowCount() > 0)
    {
        QList<QList<QVariant>> listOfVariantList;
        m_rootItem->addInListOfVariantList(-1, listOfVariantList);
        settings->setValue(m_settingsKey, QVariant::fromValue(listOfVariantList));
    }
    else if (settings->contains(m_settingsKey))
    {
        settings->remove(m_settingsKey);
    }
}

void ColumnTree::_loadFromSettings() // TODO readjust based on new columns
{
    auto settings = WorkingDirectoryManager::instance()->settings();
    auto listOfVariantList
            = settings->value(m_settingsKey).value<QList<QList<QVariant>>>();
    QHash<QString, ColumnTreeItem *> itemById;
    for (const auto &child : m_rootItem->children())
    {
        itemById[child->columnId()] = child;
    }
    QHash<QString, ColumnTreeItem *> itemLoadedById;
    QList<ColumnTreeItem *> currentItemsByLevel;
    currentItemsByLevel << new ColumnTreeItem;
    for (auto &variantList : listOfVariantList)
    {
        int level = variantList.takeFirst().toInt();
        const QString &className = variantList.takeFirst().toString();
        ColumnTreeItem *parent = currentItemsByLevel[level];
        ColumnTreeItem *child = new ColumnTreeItem(parent);
        for (int i=0; i<variantList.size(); ++i)
        {
            child->setData(i, variantList[i]);
        }
        QString columnId;
        if (level == 0)
        {
            columnId = variantList[0].toString();
            child->setColumnId(variantList[0].toString());
        }
        if (!columnId.isEmpty())
        {
            auto all = Column::ALL_COLUMNS_BY_ID;
            Q_ASSERT(Column::ALL_COLUMNS_BY_ID.contains(columnId));
            child->setName(Column::ALL_COLUMNS_BY_ID[columnId]->name());
            itemLoadedById[columnId] = child;
        }
        int nextLevel = level + 1;
        if (currentItemsByLevel.size() == nextLevel)
        {
            currentItemsByLevel << nullptr;
        }
        currentItemsByLevel[nextLevel] = child;
    }
    for (auto &child : m_rootItem->children())
    {
        const QString &columnId = child->columnId();
        if (itemLoadedById.contains(columnId))
        {
            auto subItems = itemLoadedById[columnId]->children();
            for (const auto &subItem : subItems)
            {
                new ColumnTreeItem{subItem->name(),
                            subItem->columnId(),
                            child};
            }
        }
    }
    delete currentItemsByLevel[0];
}

ColumnTree *ColumnTree::instance()
{
    static ColumnTree instance;
    return &instance;
}

ColumnTree::~ColumnTree()
{
    delete m_rootItem;
}

QSharedPointer<QStringList> ColumnTree::getPossibleColumns(
        const Column &column) const
{
    QSharedPointer<QStringList> possibleColumns{new QStringList{column.name()}};
    for (const auto &item : m_rootItem->children())
    {
        if (item->name().compare(column.name()) == 0)
        {
            if (item->name() != column.name())
            {
                *possibleColumns << item->name();
            }
            for (const auto &subItem : item->children())
            {
                *possibleColumns << subItem->name();
            }
        }
    }
    return possibleColumns;
}

QStringList ColumnTree::clearColumnsExisting(const QStringList &colNames)
{
    QStringList clearedColNames = colNames;
    for (int i=colNames.size()-1; i>=0; --i)
    {
        if (containsColumn(colNames[i]))
        {
            clearedColNames.removeAt(i);
        }
    }
    return clearedColNames;
}

QSharedPointer<QList<QStringList>> ColumnTree::getLinesFormated(
        const QStringList &colNames, const QList<QStringList> &linesFrom) const
{
    QSharedPointer<QList<QStringList>> linesFormated{new QList<QStringList>};
    int nItems = m_rootItem->childCount();
    QHash<int, int> treeToFileIndexes;
    for (int i=0; i<nItems; ++i)
    {
        auto child = m_rootItem->child(i);
        treeToFileIndexes[i] = -1;
        for (int j=0; j<colNames.size(); ++j)
        {
            if (child->equals(colNames[j]))
            {
                treeToFileIndexes[i] = j;
                break;
            }
        }
    }
    for (const auto &line : linesFrom)
    {
        QStringList newLine{treeToFileIndexes.size()};
        for (auto it = treeToFileIndexes.begin();
             it != treeToFileIndexes.end(); ++it)
        {
            newLine[it.key()] = it.value() == -1 ? QString{} : line[it.value()];
        }
        *linesFormated << newLine;
    }
    return linesFormated;
}

bool ColumnTree::containsColumn(const QString &name) const
{
    for (const auto &item : m_rootItem->children())
    {
        if (item->name().compare(name, Qt::CaseInsensitive) == 0)
        {
            return true;
        }
        for (const auto &subItem : item->children())
        {
            if (subItem->name().compare(name, Qt::CaseInsensitive) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

QStringList ColumnTree::getHeader() const
{
    QStringList header;
    for (const auto &item : m_rootItem->children())
    {
        header << item->name().toLower();
    }
    return header;
}

QHash<QString, QSet<QString> > ColumnTree::getGolNamesTree() const
{
    QHash<QString, QSet<QString> > colNames;
    for (const auto &item : m_rootItem->children())
    {
        for (const auto &subItem : item->children())
        {
            colNames[item->name()] << subItem->name();
        }
    }
    return colNames;
}

void ColumnTree::addSubItem(
        QModelIndex parentIndex, const QString &name)
{
    ColumnTreeItem *parent = nullptr;
    if (parentIndex.isValid())
    {
        parent = static_cast<ColumnTreeItem *>(
                    parentIndex.internalPointer());
        if (parent != m_rootItem)
        {
            if (!parent->isTopItem())
            {
                parentIndex = parentIndex.parent();
                parent = parent->parent();
            }
            beginInsertRows(parentIndex, parent->rowCount(), parent->rowCount());
            new ColumnTreeItem{name, QString{}, parent};
            _saveInSettings();
            endInsertRows();
        }
        else
        {
            qWarning() << "No item can be added as top item";
        }
    }
}

void ColumnTree::upItem(const QModelIndex &itemIndex)
{
    QModelIndex parentIndex = itemIndex.parent();
    ColumnTreeItem *parent;
    if (parentIndex.isValid())
    {
        parent = static_cast<ColumnTreeItem *>(
            parentIndex.internalPointer());
    }
    else
    {
        parent = m_rootItem;
    }
    int row = itemIndex.row();
    parent->up(row);
    auto index1 = index(row-1, 0, parentIndex);
    auto index2 = index(row, columnCount()-1, parentIndex);
    _saveInSettings();
    emit dataChanged(index1, index2);
}

void ColumnTree::downItem(const QModelIndex &itemIndex)
{
    QModelIndex parentIndex = itemIndex.parent();
    ColumnTreeItem *parent;
    if (parentIndex.isValid())
    {
        parent = static_cast<ColumnTreeItem *>(
            parentIndex.internalPointer());
    }
    else
    {
        parent = m_rootItem;
    }
    int row = itemIndex.row();
    parent->down(row);
    auto index1 = index(row, 0, parentIndex);
    auto index2 = index(row+1, columnCount()-1, parentIndex);
    _saveInSettings();
    emit dataChanged(index1, index2);
}

void ColumnTree::removeSubItem(const QModelIndex &itemIndex)
{
    ColumnTreeItem *parent = nullptr;
    ColumnTreeItem *child
            = static_cast<ColumnTreeItem *>(
                itemIndex.internalPointer());
    if (!child->isTopItem())
    {
        QModelIndex parentIndex = itemIndex.parent();
        parent = static_cast<ColumnTreeItem *>(
                    parentIndex.internalPointer());
        beginRemoveRows(parentIndex, child->row(), child->row());
        parent->remove(child->row());
        _saveInSettings();
        endRemoveRows();
    }
    else
    {
        qWarning() << "Top items can't be removed";
    }
}

QVariant ColumnTree::headerData(
        int, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return tr("Nom");
    }
    return QVariant{};
}

QModelIndex ColumnTree::index(
        int row, int column, const QModelIndex &parent) const
{
    QModelIndex index;
    if (hasIndex(row, column, parent))
    {
        ColumnTreeItem *item = nullptr;
        if (parent.isValid())
        {
            ColumnTreeItem *itemParent
                    = static_cast<ColumnTreeItem *>(
                        parent.internalPointer());
            item = itemParent->child(row);;
        }
        else
        {
            item = m_rootItem->child(row);
        }
        index = createIndex(row, column, item);
    }
    return index;
}

QModelIndex ColumnTree::parent(const QModelIndex &index) const
{
    QModelIndex parentIndex;
    if (index.isValid())
    {
        ColumnTreeItem *item
                = static_cast<ColumnTreeItem *>(
                    index.internalPointer());
        if (item->parent() != nullptr)
        {
            parentIndex = createIndex(
                        item->parent()->row(), 0, item->parent());
        }
    }
    return parentIndex;
}

int ColumnTree::rowCount(const QModelIndex &parent) const
{
    ColumnTreeItem *itemParent = nullptr;
    if (parent.isValid())
    {
        itemParent = static_cast<ColumnTreeItem *>(
                    parent.internalPointer());
    }
    else
    {
        itemParent = m_rootItem;
    }
    if (itemParent == nullptr)
    {
        return 0;
    }
    int count = itemParent->rowCount();
    return count;
}

int ColumnTree::columnCount(const QModelIndex &) const
{
    if (m_rootItem == nullptr)
    {
        return 0;
    }
    return m_rootItem->columnCount();
}

QVariant ColumnTree::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
            ColumnTreeItem *item
                    = static_cast<ColumnTreeItem *>(
                        index.internalPointer());
            return item->data(index.column());
        }
    }
    return QVariant();
}

bool ColumnTree::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value && role == Qt::EditRole)
    {
        ColumnTreeItem *item
                = static_cast<ColumnTreeItem *>(
                    index.internalPointer());
        item->setData(index.column(), value);
        emit dataChanged(index, index, {role});
        _saveInSettings();
        return true;
    }
    return false;
}

Qt::ItemFlags ColumnTree::flags(const QModelIndex &index) const
{
    ColumnTreeItem *item
            = static_cast<ColumnTreeItem *>(
                index.internalPointer());
    return item->flags(index.column());
}
