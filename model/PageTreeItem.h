#ifndef PAGETREEITEM_H
#define PAGETREEITEM_H

#include <QString>
#include <QVariantList>
#include <QList>

class PageTreeItem
{
public:
    static const QString CLASS_NAME;
    static const QStringList COLUMN_NAMES;
    static const int IND_EMAIL;
    static const int IND_URL;
    PageTreeItem(const QString &settingsPath,
                 const QString &email,
                 const QString &url,
                 PageTreeItem *parent = nullptr);
    PageTreeItem(const QString &settingsPath,
                 PageTreeItem *parent = nullptr);
    virtual ~PageTreeItem();
    void setContent(const QString& content);
    void setSummary(const QString& summary);
    QString getContentLoadingInNeeded();
    QString getSummaryLoadingInNeeded();
    QString getUrl() const;
    QString getEmail() const;

    PageTreeItem *child(int row) const;
    void remove(int row);
    PageTreeItem *parent() const;
    QVariant data(int column) const;
    void setData(int column, const QVariant &value);
    int rowCount() const;
    int row() const;
    virtual Qt::ItemFlags flags(int column) const;
    void addInListOfVariantList(
            int level, QList<QList<QVariant>> &listOfVariantList);
    virtual QString className() const;
    const QList<PageTreeItem *> &children() const;

protected:
    QVariantList m_values;
    PageTreeItem *m_parent;
    QList<PageTreeItem *> m_children;
    int m_row;

    bool m_contentLoaded;
    QString m_settingsPath;
    QString m_content;
    QString m_summary;

    QString _settingsKeyContent() const;
    QString _settingsKeySummary() const;
    void _loadContentIfNeeded();
    void _loadFromSettings();
    void _saveInSettings();
    void _saveInSettingsContent();
    void _saveInSettingsSummary();
};

#endif // PAGETREEITEM_H
