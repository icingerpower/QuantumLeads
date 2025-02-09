#include "ColumnTreeItem.h"

ColumnTreeItem::ColumnTreeItem(ColumnTreeItem *parent)
{
    m_parent = parent;
    m_row = 0;
    if (parent != nullptr)
    {
        m_row = m_parent->m_children.size();
        m_parent->m_children << this;
    }
    m_values << QString{};
}

bool ColumnTreeItem::equals(const QString &name) const
{
    if (this->name().compare(name, Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    for (const auto &subItem : children())
    {
        if (subItem->name().compare(name, Qt::CaseInsensitive) == 0)
        {
            return true;
        }
    }
    return false;
}

ColumnTreeItem::ColumnTreeItem(
        const QString &name, const QString &columnId, ColumnTreeItem *parent)
    : ColumnTreeItem(parent)
{
    m_columnId = columnId;
    m_values[0] = name;
}

ColumnTreeItem::~ColumnTreeItem()
{
    qDeleteAll(m_children);
}

ColumnTreeItem *ColumnTreeItem::child(int row) const
{
    return m_children[row];
}

void ColumnTreeItem::remove(int row)
{
    auto item = m_children.takeAt(row);
    for (int i=row; i<m_children.size(); ++i)
    {
        m_children[i]->m_row = i;
    }
    delete item;
}

void ColumnTreeItem::up(int row)
{
    if (row > 0)
    {
        auto temp = m_children[row-1];
        m_children[row-1] = m_children[row];
        m_children[row] = temp;
        m_children[row-1]->m_row = row-1;
        m_children[row]->m_row = row;
    }
}

void ColumnTreeItem::down(int row)
{
    if (row < m_children.size()-1)
    {
        auto temp = m_children[row+1];
        m_children[row+1] = m_children[row];
        m_children[row] = temp;
        m_children[row+1]->m_row = row+1;
        m_children[row]->m_row = row;
    }
}

ColumnTreeItem *ColumnTreeItem::parent() const
{
    return m_parent;
}

QVariant ColumnTreeItem::data(int column) const
{
    return m_values[column];
}

QString ColumnTreeItem::name() const
{
    return m_values[0].toString();
}

void ColumnTreeItem::setName(const QString &name)
{
    m_values[0] = name;
}

void ColumnTreeItem::setData(int column, const QVariant &value)
{
    m_values[column] = value;
}

int ColumnTreeItem::rowCount() const
{
    return m_children.size();
}

int ColumnTreeItem::row() const
{
    return m_row;
}

int ColumnTreeItem::columnCount() const
{
    return m_values.size();
}

Qt::ItemFlags ColumnTreeItem::flags(int) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool ColumnTreeItem::isTopItem() const
{
    return m_parent != nullptr && m_parent->m_parent == nullptr;
}

void ColumnTreeItem::addInListOfVariantList(
        int level, QList<QList<QVariant> > &listOfVariantList)
{
    if (level >= 0)
    {
        QList<QVariant> values = {level, className()};
        int colIndexName = values.size();
        for (auto it=m_values.begin(); it!=m_values.end(); ++it)
        {
            values << *it;
        }
        if (!m_columnId.isEmpty())
        {
            values[colIndexName] = m_columnId;
        }
        listOfVariantList << values;
    }
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        (*it)->addInListOfVariantList(level + 1, listOfVariantList);
    }
}

QString ColumnTreeItem::className() const
{
    return "ColumnTreeItem";
}

const QList<ColumnTreeItem *> &ColumnTreeItem::children() const
{
    return m_children;
}

int ColumnTreeItem::childCount() const
{
    return m_children.size();
}

void ColumnTreeItem::setColumnId(const QString &newColumnId)
{
    m_columnId = newColumnId;
}

const QString &ColumnTreeItem::columnId() const
{
    return m_columnId;
}

