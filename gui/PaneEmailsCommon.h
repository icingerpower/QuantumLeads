#ifndef PANEEMAILSCOMMON_H
#define PANEEMAILSCOMMON_H

#include <QWidget>
#include <QItemSelection>

namespace Ui {
class PaneEmailsCommon;
}

class PaneEmailsCommon : public QWidget
{
    Q_OBJECT

public:
    explicit PaneEmailsCommon(QWidget *parent = nullptr);
    ~PaneEmailsCommon();

public slots:
    void sendEmails(bool start);
    void addEmail();
    void editEmailContent();
    void addHeadlineVariation();
    void removeEmail();
    void onEmailSelected(
            const QItemSelection &selected, const QItemSelection &deselected);

private:
    Ui::PaneEmailsCommon *ui;
    void _connectSlots();
};

#endif // PANEEMAILSCOMMON_H
