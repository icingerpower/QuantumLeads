#include "NewColumnsList.h"

NewColumnsList::NewColumnsList(const QStringList &columnNames, QObject *parent)
    : QAbstractListModel(parent)
{
    m_colNames = columnNames;
    m_origColNames = QSet<QString>{columnNames.begin(), columnNames.end()};
}

QString NewColumnsList::remove(const QModelIndex &index)
{
    beginRemoveRows(QModelIndex{}, index.row(), index.row());
    const QString &name = m_colNames.takeAt(index.row());
    endRemoveRows();
    return name;
}

void NewColumnsList::putBack(const QString &colName)
{
    if (m_origColNames.contains(colName))
    {
        beginInsertRows(QModelIndex{}, 0, 0);
        m_colNames.insert(0, colName);
        endInsertRows();
    }
}

int NewColumnsList::rowCount(const QModelIndex &parent) const
{
    return m_colNames.size();
}

QVariant NewColumnsList::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return m_colNames[index.row()];
    }
    return QVariant();
}
