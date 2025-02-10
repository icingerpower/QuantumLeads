#include <QObject>

#include "EmailTreeItem.h"

const QString EmailTreeItem::CLASS_NAME{"EmailTreeItem"};
const QStringList EmailTreeItem::COLUMN_NAMES{QObject::tr("Subject"), QObject::tr("Email"), QObject::tr("Sent")};
const int EmailTreeItem::IND_SUBJECT{0};
const int EmailTreeItem::IND_EMAIL_SENT_TO{1};
const int EmailTreeItem::IND_DATE_TIME_SENT{2};

EmailTreeItem::EmailTreeItem(EmailTreeItem *parent)
{
    m_parent = parent;
    m_row = 0;
    if (parent != nullptr)
    {
        m_row = m_parent->m_children.size();
        m_parent->m_children << this;
    }
    for (const auto &columnName : COLUMN_NAMES)
    {
        m_values << QString{};
        m_values << QStringList{};
        m_values << QVariant::fromValue(QList<QDateTime>{});
    }
}

EmailTreeItem::EmailTreeItem(
        const QString &subject,
        const QString &content,
        EmailTreeItem *parent)
    : EmailTreeItem(parent)
{
    m_values[IND_SUBJECT] = subject;
    m_content = content;
}

QString EmailTreeItem::subject() const
{
    return m_values[IND_SUBJECT].toString();
}

void EmailTreeItem::addEmailSent(const QString &email, const QDateTime &dateTime)
{
    m_values[IND_EMAIL_SENT_TO] = m_values[IND_EMAIL_SENT_TO].toStringList() << email;
    m_values[IND_DATE_TIME_SENT]
            = QVariant::fromValue(
                m_values[IND_DATE_TIME_SENT].value<QList<QDateTime>>() << dateTime);
}

EmailTreeItem *EmailTreeItem::child(int row) const
{
    return m_children[row];
}

void EmailTreeItem::remove(int row)
{
    auto item = m_children.takeAt(row);
    for (int i=row; i<m_children.size(); ++i)
    {
        m_children[i]->m_row = i;
    }
    delete item;
}

EmailTreeItem *EmailTreeItem::parent() const
{
    return m_parent;
}

QVariant EmailTreeItem::data(int column) const
{
    if (column == IND_EMAIL_SENT_TO)
    {
        const QStringList &emails
                = m_values[IND_EMAIL_SENT_TO].toStringList();
        if (emails.size() == 0)
        {
            return QVariant{};
        }
        else if (emails.size() == 1)
        {
            return emails[0];
        }
        else
        {
            return QString::number(emails.size()) + " " + QObject::tr("emails");
        }
    }
    else if (column == IND_EMAIL_SENT_TO)
    {
        const auto &dateTimes
                = m_values[IND_DATE_TIME_SENT].value<QList<QDateTime>>();
        if (dateTimes.size() == 0)
        {
            return QVariant{};
        }
        else if (dateTimes.size() == 1)
        {
            return dateTimes[0];
        }
        else
        {
            static QString dateFormat("yyyy/mm/dd");
            return dateTimes[0].toString(dateFormat)
                    + " " + QObject::tr("to", "Date1 to date2")
                    + " " + dateTimes[0].toString(dateFormat);
;
        }
    }
    return m_values[column];
}

void EmailTreeItem::setData(int column, const QVariant &value)
{
    m_values[column] = value.toString();
}

int EmailTreeItem::rowCount() const
{
    return m_children.size();
}

int EmailTreeItem::row() const
{
    return m_row;
}

Qt::ItemFlags EmailTreeItem::flags(int column) const
{
    if (column == IND_SUBJECT)
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void EmailTreeItem::addInListOfVariantList(
        int level, QList<QList<QVariant>> &listOfVariantList)
{
    if (level >= 0)
    {
        QList<QVariant> values = {level, className()};
        for (auto it=m_values.begin(); it!=m_values.end(); ++it)
        {
            values << *it;
        }
        values << m_content;
        listOfVariantList << values;
    }
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        (*it)->addInListOfVariantList(level + 1, listOfVariantList);
    }
}

QString EmailTreeItem::className() const
{
    return CLASS_NAME;
}

const QList<EmailTreeItem *> &EmailTreeItem::children() const
{
    return m_children;
}

const QString &EmailTreeItem::content() const
{
    if (m_content.isEmpty() && m_parent != nullptr)
    {
        return m_parent->content();
    }
    return m_content;
}

void EmailTreeItem::setContent(const QString &newContent)
{
    m_content = newContent;
}
