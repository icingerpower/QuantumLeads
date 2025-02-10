#include <QObject>
#include <QSettings>

#include "PageTreeItem.h"

const QString PageTreeItem::CLASS_NAME{"PageTreeItem"};
const QStringList PageTreeItem::COLUMN_NAMES{
    QObject::tr("Email"), QObject::tr("Url")};
const int PageTreeItem::IND_EMAIL{0};
const int PageTreeItem::IND_URL{1};

PageTreeItem::PageTreeItem(
        const QString &settingsPath, PageTreeItem *parent)
{
    m_settingsPath = settingsPath;
    m_contentLoaded = false;
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
    }
}

PageTreeItem::PageTreeItem(
        const QString &settingsPath,
        const QString &email,
        const QString &url,
        PageTreeItem *parent)
    : PageTreeItem(settingsPath)
{
    m_values[IND_URL] = url;
    m_values[IND_EMAIL] = email;
}

PageTreeItem::~PageTreeItem()
{
}

void PageTreeItem::setContent(const QString &content)
{
    m_content = content;
    m_contentLoaded = true;
    _saveInSettingsContent();
}

void PageTreeItem::setSummary(const QString &summary)
{
    m_summary = summary;
    m_contentLoaded = true;
    _saveInSettingsSummary();
}

QString PageTreeItem::getContentLoadingInNeeded()
{
    _loadContentIfNeeded();
    return m_content;
}

void PageTreeItem::_loadContentIfNeeded()
{
    if (!m_contentLoaded)
    {
        _loadFromSettings();
        m_contentLoaded = true;
    }
}

void PageTreeItem::_loadFromSettings()
{
    QSettings settings{m_settingsPath, QSettings::IniFormat};
    m_content = settings.value(_settingsKeyContent(), QString{}).toString();
    m_summary = settings.value(_settingsKeySummary(), QString{}).toString();
}

void PageTreeItem::_saveInSettings()
{
    _saveInSettingsContent();
    _saveInSettingsSummary();
}

void PageTreeItem::_saveInSettingsContent()
{
    QSettings settings{m_settingsPath, QSettings::IniFormat};
    const auto &keyContent = _settingsKeyContent();
    if (!m_content.isEmpty())
    {
        settings.setValue(keyContent, m_content);
    }
    else if (settings.contains(keyContent))
    {
        settings.remove(keyContent);
    }
}

void PageTreeItem::_saveInSettingsSummary()
{
    QSettings settings{m_settingsPath, QSettings::IniFormat};
    const auto &keySummary = _settingsKeySummary();
    if (!m_summary.isEmpty())
    {
        settings.setValue(keySummary, m_summary);
    }
    else if (settings.contains(keySummary))
    {
        settings.remove(keySummary);
    }
}

QString PageTreeItem::getSummaryLoadingInNeeded()
{
    _loadContentIfNeeded();
    return m_summary;
}

QString PageTreeItem::getUrl() const
{
    return m_values[IND_URL].toString();
}

QString PageTreeItem::getEmail() const
{
    return m_values[IND_EMAIL].toString();
}

PageTreeItem *PageTreeItem::child(int row) const
{
    return m_children[row];
}

void PageTreeItem::remove(int row)
{
    auto item = m_children.takeAt(row);
    for (int i=row; i<m_children.size(); ++i)
    {
        m_children[i]->m_row = i;
    }
    delete item;
}

PageTreeItem *PageTreeItem::parent() const
{
    return m_parent;
}

QVariant PageTreeItem::data(int column) const
{
    return m_values[column];
}

void PageTreeItem::setData(int column, const QVariant &value)
{
    m_values[column] = value.toString();
}

int PageTreeItem::rowCount() const
{
    return m_children.size();
}

int PageTreeItem::row() const
{
    return m_row;
}

Qt::ItemFlags PageTreeItem::flags(int column) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void PageTreeItem::addInListOfVariantList(int level, QList<QList<QVariant> > &listOfVariantList)
{
    if (level >= 0)
    {
        QList<QVariant> values = {level, className()};
        for (auto it=m_values.begin(); it!=m_values.end(); ++it)
        {
            values << *it;
        }
        listOfVariantList << values;
    }
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        (*it)->addInListOfVariantList(level + 1, listOfVariantList);
    }
}

QString PageTreeItem::className() const
{
    return CLASS_NAME;
}

const QList<PageTreeItem *> &PageTreeItem::children() const
{
    return m_children;
}

QString PageTreeItem::_settingsKeyContent() const
{
    return getUrl().simplified().replace(" ", "");
}

QString PageTreeItem::_settingsKeySummary() const
{
    return "summary" + _settingsKeyContent();
}

