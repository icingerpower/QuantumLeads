#ifndef COLUMNTREEITEM_H
#define COLUMNTREEITEM_H

#include <QString>
#include <QVariant>


class ColumnTreeItem
{
public:
    static int IND_COL_NAME;
    static int IND_COL_LINK;
    static int IND_COL_FOLLOW;
    static int IND_COL_NEW_TAB;
    static int IND_COL_LAST;
    static QString CLASS_NAME;
    ColumnTreeItem(
            const QString &name,
            const QString &columnId,
            ColumnTreeItem *parent = nullptr);
    ColumnTreeItem(
            ColumnTreeItem *parent = nullptr);
    bool equals(const QString &name) const;
    virtual ~ColumnTreeItem();
    ColumnTreeItem *child(int row) const;
    void remove(int row);
    void up(int row);
    void down(int row);
    ColumnTreeItem *parent() const;
    QVariant data(int column) const;
    QString name() const;
    void setName(const QString &name);
    void setData(int column, const QVariant &value);
    int rowCount() const;
    int row() const;
    int columnCount() const;
    virtual Qt::ItemFlags flags(int column) const;
    bool isTopItem() const;

    void addInListOfVariantList(
            int level, QList<QList<QVariant>> &listOfVariantList);
    virtual QString className() const;
    const QList<ColumnTreeItem *> &children() const;
    int childCount() const;


    void setColumnId(const QString &newColumnId);

    const QString &columnId() const;

protected:
    QString m_columnId;
    QList<QVariant> m_values;
    ColumnTreeItem *m_parent;
    QList<ColumnTreeItem *> m_children;
    int m_row;
};



#endif // COLUMNTREEITEM_H
