#include <QBrush>

#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "Column.h"

#include "LeadsTableModel.h"

LeadsTableModel::LeadsTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_settingsKey = "LeadsTable";
    m_settingsKeyEmailSent = m_settingsKey + "Sent";
    m_settingsKeyEmailSentInteraction = m_settingsKey + "SentInveraction";
    m_settingsKeyEmailSentPurchase = m_settingsKey + "SentPurchase";
    _loadFromSettings();
}

LeadsTableModel *LeadsTableModel::instance()
{
    static LeadsTableModel instance;
    return &instance;
}

void LeadsTableModel::addLines(const QList<QStringList> &lines)
{
    beginInsertRows(QModelIndex{},
                    m_listOfStringList.size(),
                    m_listOfStringList.size() + lines.size() - 1);
    m_listOfStringList << lines;
    _saveInSettings();
    endInsertRows();
}

void LeadsTableModel::addEmailsSent(const QStringList &emails)
{
    for (const auto &email : emails)
    {
        m_emailSent.insert(email);
    }
    _saveInSettings();
}

void LeadsTableModel::addEmailsSentInteraction(const QStringList &emails)
{
    for (const auto &email : emails)
    {
        m_emailSentInteraction.insert(email);
    }
    _saveInSettings();
}

void LeadsTableModel::addEmailsSentPurchase(const QStringList &emails)
{
    for (const auto &email : emails)
    {
        m_emailSentPurchase.insert(email);
    }
    _saveInSettings();
}

void LeadsTableModel::removeAt(int rowIndex)
{
    beginRemoveRows(QModelIndex{}, rowIndex, rowIndex);
    m_listOfStringList.removeAt(rowIndex);
    _saveInSettings();
    endRemoveRows();
}

QVariant LeadsTableModel::headerData(
        int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return Column::ALL_COLUMNS[section].name();
    }
    return QVariant{};
}

int LeadsTableModel::rowCount(const QModelIndex &) const
{
    return m_listOfStringList.size();
}

int LeadsTableModel::columnCount(const QModelIndex &) const
{
    return Column::ALL_COLUMNS.size();
}

QVariant LeadsTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return m_listOfStringList[index.row()][index.column()];
    }
    else if (role == Qt::BackgroundRole)
    {
        static const int emailColIndex = Column::GET_COL_INDEX(
                    Column::COL_EMAIL.id());
        QString email = index.siblingAtColumn(emailColIndex).data().toString();
        if (m_emailSentPurchase.contains(email))
        {
            return QBrush{Qt::green};
        }
        else if (m_emailSentInteraction.contains(email))
        {
            return QBrush{Qt::yellow};
        }
        else if (m_emailSent.contains(email))
        {
            return QBrush{Qt::blue};
        }
    }
    return QVariant();
}

Qt::ItemFlags LeadsTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void LeadsTableModel::_loadFromSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    m_listOfStringList
            = settings->value(m_settingsKey).value<QList<QStringList>>();
    m_emailSent
            = settings->value(m_settingsKeyEmailSent).value<QSet<QString>>();
    m_emailSentInteraction
            = settings->value(m_settingsKeyEmailSentInteraction).value<QSet<QString>>();
    m_emailSentPurchase
            = settings->value(m_settingsKeyEmailSentPurchase).value<QSet<QString>>();
}

void LeadsTableModel::_saveInSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    settings->setValue(m_settingsKey,
                       QVariant::fromValue(m_listOfStringList));
    settings->setValue(m_settingsKeyEmailSent,
                       QVariant::fromValue(m_emailSent));
    settings->setValue(m_settingsKeyEmailSentInteraction,
                       QVariant::fromValue(m_emailSentPurchase));
    settings->setValue(m_settingsKeyEmailSentPurchase,
                       QVariant::fromValue(m_emailSentInteraction));
}

