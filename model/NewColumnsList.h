#ifndef NEWCOLUMNSLIST_H
#define NEWCOLUMNSLIST_H

#include <QAbstractListModel>

class NewColumnsList : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit NewColumnsList(const QStringList &columnNames, QObject *parent = nullptr);
    QString remove(const QModelIndex &index);
    void putBack(const QString &colName);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QStringList m_colNames;
    QSet<QString> m_origColNames;
};

#endif // NEWCOLUMNSLIST_H
