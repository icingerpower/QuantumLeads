#ifndef COLUMNTREE_H
#define COLUMNTREE_H

#include <QAbstractItemModel>
#include <QSharedPointer>

#include "Column.h"

class ColumnTreeItem;

class ColumnTree : public QAbstractItemModel
{
    Q_OBJECT

public:
    static ColumnTree *instance();
    static const QStringList COL_NAMES_STANDARD;
    ~ColumnTree() override;
    static QString createId(const QString &templateId);

    QSharedPointer<QStringList> getPossibleColumns(const Column &column) const;
    QStringList clearColumnsExisting(const QStringList &colNames);
    QSharedPointer<QList<QStringList>> getLinesFormated(
            const QStringList &colNames,
            const QList<QStringList> &linesFrom) const;

    bool containsColumn(const QString &name) const;
    QStringList getHeader() const;
    QHash<QString, QSet<QString>> getGolNamesTree() const;
    void addSubItem(QModelIndex parent, const QString &name);
    void upItem(const QModelIndex &itemIndex);
    void downItem(const QModelIndex &itemIndex);
    void removeSubItem(const QModelIndex &itemIndex);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    explicit ColumnTree(QObject *parent = nullptr);
    ColumnTreeItem *m_rootItem;
    void _clear();
    void _saveInSettings() const;
    void _loadFromSettings();
    QString m_settingsKey;
};

#endif // COLUMNTREE_H
