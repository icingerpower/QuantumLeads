#ifndef DIALOGADDEMAIL_H
#define DIALOGADDEMAIL_H

#include <QDialog>

namespace Ui {
class DialogAddEmail;
}

class DialogAddEmail : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddEmail(QWidget *parent = nullptr);
    ~DialogAddEmail();
    void setSubject(const QString &subject);
    void setEmailContent(const QString &email);
    QString getSubject() const;
    QString getEmailContent() const;
    void setSubjectReadOnly(bool readOnly);

public slots:
    void accept() override;


private:
    Ui::DialogAddEmail *ui;
};

#endif // DIALOGADDEMAIL_H
