#ifndef EMAILTREEVIEW_H
#define EMAILTREEVIEW_H

#include <QAbstractItemModel>

class EmailTreeItem;

class EmailTree : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit EmailTree(
            const QString &settingsFilePath,
            const QString &postSettingsKey,
            QObject *parent = nullptr);
    ~EmailTree();
    static EmailTree *instance();
    void addEmail(const QString &subject, const QString &content);
    bool addEmailVariation(const QModelIndex &parent, const QString &subject);
    QString getSubject(const QModelIndex &index) const;
    QString getEmailContent(const QModelIndex &index) const;
    void setEmailContent(const QModelIndex &index, const QString &content);
    void removeEmail(const QModelIndex &index);

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
    QString m_settingsFilePath;
    QString m_settingsKey;
    void _clear();
    void _saveInSettings() const;
    void _loadFromSettings();
    EmailTreeItem *m_rootItem;
};

#endif // EMAILTREEVIEW_H
