#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "PageTreeItem.h"

#include "PageTree.h"

const QString PageTree::SETTINGS_FILE_CONTENT{"webpages.ini"};
const QString PageTree::SETTINGS_KEY{"WebPageUrls"};

PageTree::PageTree(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_settingsFilePathWebContent
            = WorkingDirectoryManager::instance()->settingsFilePath(SETTINGS_FILE_CONTENT);

    m_rootItem = new PageTreeItem{m_settingsFilePathWebContent};
    _loadFromSettings();
}

void PageTree::_clear()
{
    if (m_rootItem != nullptr)
    {
        beginRemoveRows(QModelIndex(), 0, m_rootItem->rowCount());
        delete m_rootItem;
        m_rootItem = new PageTreeItem{m_settingsFilePathWebContent};
        endRemoveRows();
    }
    else
    {
        m_rootItem = new PageTreeItem{m_settingsFilePathWebContent};
    }
}

void PageTree::_loadFromSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settings();
    auto listOfVariantList
            = settings->value(SETTINGS_KEY).value<QList<QList<QVariant>>>();
    _clear();
    QList<PageTreeItem *> currentItemsByLevel;
    currentItemsByLevel << m_rootItem;
    for (auto &variantList : listOfVariantList)
    {
        int level = variantList.takeFirst().toInt();
        const QString &className = variantList.takeFirst().toString();
        PageTreeItem *parent = currentItemsByLevel[level];
        PageTreeItem *child = new PageTreeItem(m_settingsFilePathWebContent, parent);
        for (int i=0; i<variantList.size(); ++i)
        {
            child->setData(i, variantList[i]);
        }
        int nextLevel = level + 1;
        if (currentItemsByLevel.size() == nextLevel)
        {
            currentItemsByLevel << nullptr;
        }
        currentItemsByLevel[nextLevel] = child;
    }
}

void PageTree::_saveInSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settings();
    if (rowCount() > 0)
    {
        QList<QList<QVariant>> listOfVariantList;
        m_rootItem->addInListOfVariantList(-1, listOfVariantList);
        settings->setValue(SETTINGS_KEY, QVariant::fromValue(listOfVariantList));
    }
    else if (settings->contains(SETTINGS_KEY))
    {
        settings->remove(SETTINGS_KEY);
    }
}

PageTree *PageTree::instance()
{
    static PageTree instance;
    return &instance;
}

PageTree::~PageTree()
{
    delete m_rootItem;
}

void PageTree::addUrl(const QString &email,
        const QString &url, const QString &content, const QString &summary)
{
    beginInsertRows(QModelIndex{}, rowCount(), rowCount());
    auto pageTreeItem = new PageTreeItem{
            m_settingsFilePathWebContent, email, url, m_rootItem};
    pageTreeItem->setContent(content);
    pageTreeItem->setContent(summary);
    _saveInSettings();
    endInsertRows();
}

QVariant PageTree::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return PageTreeItem::COLUMN_NAMES[section];
    }
    return QVariant{};
}

QModelIndex PageTree::index(
        int row, int column, const QModelIndex &parent) const
{
    QModelIndex index;
    if (hasIndex(row, column, parent))
    {
        PageTreeItem *item = nullptr;
        if (parent.isValid())
        {
            PageTreeItem *itemParent
                    = static_cast<PageTreeItem *>(
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

QModelIndex PageTree::parent(const QModelIndex &index) const
{
    QModelIndex parentIndex;
    if (index.isValid())
    {
        PageTreeItem *item
                = static_cast<PageTreeItem *>(
                    index.internalPointer());
        if (item->parent() != nullptr)
        {
            parentIndex = createIndex(
                        item->parent()->row(), 0, item->parent());
        }
    }
    return parentIndex;

}

int PageTree::rowCount(const QModelIndex &parent) const
{
    PageTreeItem *itemParent = nullptr;
    if (parent.isValid())
    {
        itemParent = static_cast<PageTreeItem *>(
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

int PageTree::columnCount(const QModelIndex &) const
{
    return PageTreeItem::COLUMN_NAMES.size();
}

QVariant PageTree::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
            PageTreeItem *item
                    = static_cast<PageTreeItem *>(
                        index.internalPointer());
            return item->data(index.column());
        }
    }
    return QVariant();
}

