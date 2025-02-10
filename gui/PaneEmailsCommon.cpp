#include <QMessageBox>
#include <QInputDialog>

#include "gui/DialogAddEmail.h"
#include "model/EmailTree.h"

#include "PaneEmailsCommon.h"
#include "ui_PaneEmailsCommon.h"

PaneEmailsCommon::PaneEmailsCommon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneEmailsCommon)
{
    ui->setupUi(this);
    ui->treeViewEmails->setModel(EmailTree::instance());
    ui->treeViewEmails->expandAll();
    _connectSlots();
}

void PaneEmailsCommon::_connectSlots()
{
    connect(ui->buttonSendEmails,
            &QPushButton::clicked,
            this,
            &PaneEmailsCommon::sendEmails);
    connect(ui->buttonAddEmail,
            &QPushButton::clicked,
            this,
            &PaneEmailsCommon::addEmail);
    connect(ui->buttonEditEmailContent,
            &QPushButton::clicked,
            this,
            &PaneEmailsCommon::editEmailContent);
    connect(ui->buttonAddHeadline,
            &QPushButton::clicked,
            this,
            &PaneEmailsCommon::addHeadlineVariation);
    connect(ui->buttonRemove,
            &QPushButton::clicked,
            this,
            &PaneEmailsCommon::removeEmail);
    connect(ui->treeViewEmails->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &PaneEmailsCommon::onEmailSelected);
}

PaneEmailsCommon::~PaneEmailsCommon()
{
    delete ui;
}

void PaneEmailsCommon::sendEmails(bool start)
{

}

void PaneEmailsCommon::addEmail()
{
    DialogAddEmail dialog;
    dialog.exec();
    if (dialog.result() == QDialog::Accepted)
    {
        EmailTree::instance()->addEmail(
                    dialog.getSubject(), dialog.getEmailContent());
    }

}

void PaneEmailsCommon::editEmailContent()
{
    const auto &selIndexes = ui->treeViewEmails->selectionModel()->selectedIndexes();
    if (selIndexes.size() > 0)
    {
        auto firstIndex = selIndexes.first();
        if (firstIndex.parent().parent().isValid())
        {
            firstIndex = firstIndex.parent();
        }
        DialogAddEmail dialog;
        dialog.setSubject(EmailTree::instance()->getSubject(firstIndex));
        dialog.setEmailContent(EmailTree::instance()->getEmailContent(firstIndex));
        dialog.exec();
        if (dialog.result() == QDialog::Accepted)
        {
            const QString &emailContent = dialog.getEmailContent();
            EmailTree::instance()->setEmailContent(firstIndex, dialog.getEmailContent());
            ui->textEditEmail->setPlainText(emailContent);
        }
    }
    else
    {
        QMessageBox::information(
                    this,
                    tr("No selection"),
                    tr("You need to select an email to edit."));
    }
}

void PaneEmailsCommon::addHeadlineVariation()
{
    const auto &selIndexes = ui->treeViewEmails->selectionModel()->selectedIndexes();
    if (selIndexes.size() > 0)
    {
        auto firstIndex = selIndexes.first();
        if (firstIndex.parent().parent().isValid())
        {
            firstIndex = firstIndex.parent();
        }
        const QString &subject = QInputDialog::getText(
                    this,
                    tr("Subject"),
                    tr("Enter the email subject"));
        EmailTree::instance()->addEmailVariation(
                    firstIndex, subject);
        ui->treeViewEmails->expand(firstIndex);
    }
}

void PaneEmailsCommon::removeEmail()
{
    const auto &selIndexes = ui->treeViewEmails->selectionModel()->selectedIndexes();
    if (selIndexes.size() > 0)
    {
        auto firstIndex = selIndexes.first();
        EmailTree::instance()->removeEmail(firstIndex);
    }
}

void PaneEmailsCommon::onEmailSelected(
        const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.size() > 0)
    {
        auto firstIndex = selected.indexes().first();
        ui->lineEditEmail->setText(EmailTree::instance()->getSubject(firstIndex));
        ui->textEditEmail->setPlainText(EmailTree::instance()->getEmailContent(firstIndex));
    }
    else if (deselected.size() > 0)
    {
        ui->lineEditEmail->clear();
        ui->textEditEmail->clear();
    }
}

