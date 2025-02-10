#ifndef PAGETREE_H
#define PAGETREE_H

#include <QAbstractItemModel>

class PageTreeItem;

class PageTree : public QAbstractItemModel
{
    Q_OBJECT

public:
    static const QString SETTINGS_FILE_CONTENT;
    static PageTree *instance();
    ~PageTree();

    void addUrl(const QString &email,
                const QString &url,
                const QString &content,
                const QString &summary);

    QVariant headerData(
            int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    static const QString SETTINGS_KEY;
    explicit PageTree(QObject *parent = nullptr);
    PageTreeItem *m_rootItem;
    QString m_settingsFilePathWebContent;
    void _clear();
    void _loadFromSettings();
    void _saveInSettings();
};

#endif // PAGETREE_H
