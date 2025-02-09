#ifndef LEADSTABLEMODEL_H
#define LEADSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QSet>

class LeadsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    static LeadsTableModel *instance();
    void addLines(const QList<QStringList> &lines);
    void addEmailsSent(const QStringList &emails);
    void addEmailsSentInteraction(const QStringList &emails);
    void addEmailsSentPurchase(const QStringList &emails);
    void removeAt(int rowIndex);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    explicit LeadsTableModel(QObject *parent = nullptr);
    void _loadFromSettings();
    void _saveInSettings();
    QList<QStringList> m_listOfStringList;
    QSet<QString> m_emailSent;
    QSet<QString> m_emailSentInteraction;
    QSet<QString> m_emailSentPurchase;
    QString m_settingsKey;
    QString m_settingsKeyEmailSent;
    QString m_settingsKeyEmailSentInteraction;
    QString m_settingsKeyEmailSentPurchase;
};

#endif // LEADSTABLEMODEL_H
