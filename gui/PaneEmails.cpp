#include "PaneEmails.h"
#include "ui_PaneEmails.h"

PaneEmails::PaneEmails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneEmails)
{
    ui->setupUi(this);
}

PaneEmails::~PaneEmails()
{
    delete ui;
}
