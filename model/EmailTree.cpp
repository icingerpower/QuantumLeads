#include <QSettings>

#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "EmailTreeItem.h"

#include "EmailTree.h"

EmailTree::EmailTree(
        const QString &settingsFilePath,
        const QString &postSettingsKey,
        QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new EmailTreeItem;
    m_settingsKey = "EmailTree" + postSettingsKey;
    m_settingsFilePath = settingsFilePath;
    _loadFromSettings();
}

EmailTree::~EmailTree()
{
    delete m_rootItem;
}

EmailTree *EmailTree::instance()
{
    static EmailTree instance{
        WorkingDirectoryManager::instance()->settingsFilePath(),
                QString{}
    };
    return &instance;
}

void EmailTree::addEmail(const QString &subject, const QString &content)
{
    beginInsertRows(QModelIndex{}, m_rootItem->rowCount(), m_rootItem->rowCount());
    new EmailTreeItem(subject, content, m_rootItem);
    _saveInSettings();
    endInsertRows();
}

bool EmailTree::addEmailVariation(
        const QModelIndex &parent, const QString &subject)
{
    EmailTreeItem *item = nullptr;
    if (parent.isValid())
    {
        EmailTreeItem *itemParent
                = static_cast<EmailTreeItem *>(
                    parent.internalPointer());
        if (itemParent != m_rootItem && itemParent->parent() == m_rootItem)
        {
            beginInsertRows(parent, itemParent->rowCount(), itemParent->rowCount());
            new EmailTreeItem(subject, QString{}, itemParent);
            _saveInSettings();
            endInsertRows();
            return true;
        }
    }
    return false;
}

QString EmailTree::getSubject(const QModelIndex &index) const
{
    if (index.isValid())
    {
        EmailTreeItem *item
                = static_cast<EmailTreeItem *>(
                    index.internalPointer());
        return item->subject();
    }
    return QString{};
}

QString EmailTree::getEmailContent(const QModelIndex &index) const
{
    if (index.isValid())
    {
        EmailTreeItem *item
                = static_cast<EmailTreeItem *>(
                    index.internalPointer());
        return item->content();
    }
    return QString{};
}

void EmailTree::setEmailContent(const QModelIndex &index, const QString &content)
{
    if (index.isValid())
    {
        EmailTreeItem *item
                = static_cast<EmailTreeItem *>(
                    index.internalPointer());
        item->setContent(content);
        _saveInSettings();
    }
}

void EmailTree::removeEmail(const QModelIndex &index)
{
    if (index.isValid())
    {
        EmailTreeItem *item
                = static_cast<EmailTreeItem *>(
                    index.internalPointer());
        QModelIndex parent = index.parent();
        beginRemoveRows(parent, item->row(), item->row());
        item->parent()->remove(item->row());
        _saveInSettings();
        endRemoveRows();
    }
}

QVariant EmailTree::headerData(
        int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return EmailTreeItem::COLUMN_NAMES[section];
    }
    return QVariant{};
}

QModelIndex EmailTree::index(int row, int column, const QModelIndex &parent) const
{
    QModelIndex index;
    if (hasIndex(row, column, parent))
    {
        EmailTreeItem *item = nullptr;
        if (parent.isValid())
        {
            EmailTreeItem *itemParent
                    = static_cast<EmailTreeItem *>(
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

QModelIndex EmailTree::parent(const QModelIndex &index) const
{
    QModelIndex parentIndex;
    if (index.isValid())
    {
        EmailTreeItem *item
                = static_cast<EmailTreeItem *>(
                    index.internalPointer());
        if (item->parent() != nullptr)
        {
            parentIndex = createIndex(
                        item->parent()->row(), 0, item->parent());
        }
    }
    return parentIndex;

}

int EmailTree::rowCount(const QModelIndex &parent) const
{
    EmailTreeItem *itemParent = nullptr;
    if (parent.isValid())
    {
        itemParent = static_cast<EmailTreeItem *>(
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

int EmailTree::columnCount(const QModelIndex &parent) const
{
    return EmailTreeItem::COLUMN_NAMES.size();
}

QVariant EmailTree::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
            EmailTreeItem *item
                    = static_cast<EmailTreeItem *>(
                        index.internalPointer());
            return item->data(index.column());
        }
    }
    return QVariant();
}

bool EmailTree::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        EmailTreeItem *item
                = static_cast<EmailTreeItem *>(
                    index.internalPointer());
        item->setData(index.column(), value);
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags EmailTree::flags(const QModelIndex &index) const
{
    EmailTreeItem *item
            = static_cast<EmailTreeItem *>(
                index.internalPointer());
    return item->flags(index.column());
}

void EmailTree::_clear()
{
    if (m_rootItem != nullptr)
    {
        beginRemoveRows(QModelIndex(), 0, m_rootItem->rowCount());
        delete m_rootItem;
        m_rootItem = new EmailTreeItem;
        endRemoveRows();
    }
    else
    {
        m_rootItem = new EmailTreeItem;
    }
}

void EmailTree::_saveInSettings() const
{
    QSettings settings(m_settingsFilePath, QSettings::IniFormat);
    if (rowCount() > 0)
    {
        QList<QList<QVariant>> listOfVariantList;
        m_rootItem->addInListOfVariantList(-1, listOfVariantList);
        settings.setValue(m_settingsKey, QVariant::fromValue(listOfVariantList));
    }
    else if (settings.contains(m_settingsKey))
    {
        settings.remove(m_settingsKey);
    }
}

void EmailTree::_loadFromSettings()
{
    QSettings settings(m_settingsFilePath, QSettings::IniFormat);
    auto listOfVariantList
            = settings.value(m_settingsKey).value<QList<QList<QVariant>>>();
    _clear();
    QList<EmailTreeItem *> currentItemsByLevel;
    currentItemsByLevel << m_rootItem;
    for (auto &variantList : listOfVariantList)
    {
        int level = variantList.takeFirst().toInt();
        //QString className = variantList.takeFirst().toString();
        variantList.takeFirst();
        QString content = variantList.takeLast().toString();
        EmailTreeItem *parent = currentItemsByLevel[level];
        EmailTreeItem *child = new EmailTreeItem(parent);
        for (int i=0; i<variantList.size(); ++i)
        {
            child->setData(i, variantList[i]);
        }
        child->setContent(content);
        int nextLevel = level + 1;
        if (currentItemsByLevel.size() == nextLevel)
        {
            currentItemsByLevel << nullptr;
        }
        currentItemsByLevel[nextLevel] = child;
    }
}
