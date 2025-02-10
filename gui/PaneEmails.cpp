#include "model/PageTree.h"

#include "PaneEmails.h"
#include "ui_PaneEmails.h"

PaneEmails::PaneEmails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneEmails)
{
    ui->setupUi(this);
    ui->treeViewUrls->setModel(PageTree::instance());
}

PaneEmails::~PaneEmails()
{
    delete ui;
}
