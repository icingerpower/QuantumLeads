#include <QMessageBox>

#include "DialogAddEmail.h"
#include "ui_DialogAddEmail.h"

DialogAddEmail::DialogAddEmail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddEmail)
{
    ui->setupUi(this);
}

DialogAddEmail::~DialogAddEmail()
{
    delete ui;
}

void DialogAddEmail::setSubject(const QString &subject)
{
    ui->lineEditSubject->setText(subject);
}

void DialogAddEmail::setEmailContent(const QString &email)
{
    ui->plainTextEdit->setPlainText(email);
}

QString DialogAddEmail::getSubject() const
{
    return ui->lineEditSubject->text();
}

void DialogAddEmail::setSubjectReadOnly(bool readOnly)
{
    ui->lineEditSubject->setReadOnly(readOnly);
}

QString DialogAddEmail::getEmailContent() const
{
    return ui->plainTextEdit->toPlainText();
}

void DialogAddEmail::accept()
{
    if (ui->lineEditSubject->text().trimmed().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("No subject"),
                    tr("You need to enter a subject for the email."));

    }
    else if (ui->plainTextEdit->toPlainText().trimmed().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("No content"),
                    tr("You need to enter a content for the email."));
    }
    else
    {
        QDialog::accept();
    }
}
