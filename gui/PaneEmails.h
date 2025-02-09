#ifndef PANEEMAILS_H
#define PANEEMAILS_H

#include <QWidget>

namespace Ui {
class PaneEmails;
}

class PaneEmails : public QWidget
{
    Q_OBJECT

public:
    explicit PaneEmails(QWidget *parent = nullptr);
    ~PaneEmails();

private:
    Ui::PaneEmails *ui;
};

#endif // PANEEMAILS_H
