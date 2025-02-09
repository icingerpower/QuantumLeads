#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidgetMenu->setCurrentRow(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

