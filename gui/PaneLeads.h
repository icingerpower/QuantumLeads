#ifndef PANELEADS_H
#define PANELEADS_H

#include <QWidget>

namespace Ui {
class PaneLeads;
}

class PaneLeads : public QWidget
{
    Q_OBJECT

public:
    explicit PaneLeads(QWidget *parent = nullptr);
    ~PaneLeads();

public slots:
    void importLeads();
    void importLeadEmailsInteraction();
    void importLeadEmailsPurchase();
    void removeLeads();

private:
    Ui::PaneLeads *ui;
    void _connectSlots();
    void _importCsvFile(const QString &filePath);
    void _importExcelFile(const QString &filePath);
    void _processColNames(const QStringList &colNames);
};

#endif // PANELEADS_H
