#include <QMessageBox>

#include "model/ColumnTree.h"
#include "model/NewColumnsList.h"

#include "DialogFileColumns.h"
#include "ui_DialogFileColumns.h"

const QString DialogFileColumns::TEXT_NO_SELECTION{QObject::tr("No selection")};

DialogFileColumns::DialogFileColumns(
        NewColumnsList *newColumns, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFileColumns)
{
    ui->setupUi(this);
    ui->treeViewColumns->setModel(ColumnTree::instance());
    ui->listViewColumns->setModel(newColumns);
    _connectSlots();
}

DialogFileColumns::~DialogFileColumns()
{
    delete ui;
}

NewColumnsList *DialogFileColumns::newColumnsListModel() const
{
    return static_cast<NewColumnsList *>(ui->listViewColumns->model());
}

void DialogFileColumns::addInTree()
{
    auto selIndexes = ui->listViewColumns->selectionModel()->selectedIndexes();
    if (selIndexes.size() > 0)
    {
        auto selTreeIndexes = ui->treeViewColumns->selectionModel()->selectedIndexes();
        if (selTreeIndexes.size() > 0)
        {
            auto newListModel = newColumnsListModel();
            const QString &colName = newListModel->remove(selIndexes.first());
            const auto &parent = selTreeIndexes.first();
            ColumnTree::instance()->addSubItem(parent, colName);
            ui->treeViewColumns->expand(parent);
        }
        else
        {
            QMessageBox::information(this,
                                     TEXT_NO_SELECTION,
                                     tr("You need to select an item from the left column tree"));
        }
    }
    else
    {
        QMessageBox::information(this,
                                 TEXT_NO_SELECTION,
                                 tr("You need to select an item from the column list"));
    }
}

void DialogFileColumns::removeFromTree()
{
    auto selIndexes = ui->treeViewColumns->selectionModel()->selectedIndexes();
    if (selIndexes.size() > 0)
    {
        const auto &indexFirst = selIndexes.first();
        const QString &colName = indexFirst.data().toString();
        auto newListModel = newColumnsListModel();
        ColumnTree::instance()->removeSubItem(indexFirst);
        newListModel->putBack(colName);
    }
    else
    {
        QMessageBox::information(this,
                                 TEXT_NO_SELECTION,
                                 tr("You need to select an item from the left column tree"));
    }
}

void DialogFileColumns::_connectSlots()
{
    connect(ui->buttonAddInTree,
            &QPushButton::clicked,
            this,
            &DialogFileColumns::addInTree);
    connect(ui->buttonRemoveFromTree,
            &QPushButton::clicked,
            this,
            &DialogFileColumns::removeFromTree);
}
