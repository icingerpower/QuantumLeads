#ifndef EMAILTREEITEM_H
#define EMAILTREEITEM_H

#include <QString>
#include <QVariantList>
#include <QList>
#include <QDateTime>

class EmailTreeItem
{
public:
    static const QString CLASS_NAME;
    static const QStringList COLUMN_NAMES;
    static const int IND_SUBJECT;
    static const int IND_EMAIL_SENT_TO;
    static const int IND_DATE_TIME_SENT;
    EmailTreeItem(EmailTreeItem *parent = nullptr);
    EmailTreeItem(const QString &subject,
                 const QString &content,
                 EmailTreeItem *parent = nullptr);

    QString subject() const;
    void addEmailSent(const QString &email, const QDateTime &dateTime);
    const QString &content() const;
    void setContent(const QString &newContent);

    EmailTreeItem *child(int row) const;
    void remove(int row);
    EmailTreeItem *parent() const;
    QVariant data(int column) const;
    void setData(int column, const QVariant &value);
    int rowCount() const;
    int row() const;
    virtual Qt::ItemFlags flags(int column) const;
    void addInListOfVariantList(
            int level, QList<QList<QVariant>> &listOfVariantList);
    virtual QString className() const;
    const QList<EmailTreeItem *> &children() const;


protected:
    QVariantList m_values;
    EmailTreeItem *m_parent;
    QList<EmailTreeItem *> m_children;
    int m_row;
    QString m_content;
};

#endif // EMAILTREEITEM_H
