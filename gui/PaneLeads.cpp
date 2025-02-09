#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include <xlsxdocument.h>

#include "../common/utils/CsvReader.h"

#include "model/ColumnTree.h"
#include "model/NewColumnsList.h"
#include "model/LeadsTableModel.h"

#include "DialogFileColumns.h"

#include "PaneLeads.h"
#include "ui_PaneLeads.h"

PaneLeads::PaneLeads(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneLeads)
{
    ui->setupUi(this);
    ui->tableViewCustomers->setModel(LeadsTableModel::instance());
    _connectSlots();
}

void PaneLeads::_connectSlots()
{
    connect(ui->buttonImportLeads,
            &QPushButton::clicked,
            this,
            &PaneLeads::importLeads);
    connect(ui->buttonImportLeadsInteraction,
            &QPushButton::clicked,
            this,
            &PaneLeads::importLeadEmailsInteraction);
    connect(ui->buttonImportLeadsPurchase,
            &QPushButton::clicked,
            this,
            &PaneLeads::importLeadEmailsPurchase);
    connect(ui->buttonRemoveLeads,
            &QPushButton::clicked,
            this,
            &PaneLeads::removeLeads);
}

PaneLeads::~PaneLeads()
{
    delete ui;
}

void PaneLeads::importLeads()
{
    QSettings settings;
    const QString key{"PaneLeads__importLeads"};
    QDir lastDir{settings.value(key, QDir{}.path()).toString()};
    const QString &filePath = QFileDialog::getOpenFileName(
                this,
                tr("Leads file"),
                lastDir.path(),
                QString{"Xlsx or csv (*.csv *.CSV *.tab *.TAB *.txt *.TXT *.xlsx *.XLSX)"},
                nullptr,
                QFileDialog::DontUseNativeDialog);
    if (!filePath.isEmpty())
    {
        settings.setValue(key, QFileInfo{filePath}.dir().path());
        if (filePath.endsWith(".xlsx", Qt::CaseInsensitive))
        {
            _importExcelFile(filePath);
        }
        else
        {
            _importCsvFile(filePath);
        }
    }
}

void PaneLeads::_importCsvFile(const QString &filePath)
{
    auto seps = CsvReader::guessColStringSeps(filePath);
    CsvReader reader{filePath,
                seps.first,
                seps.second};
    reader.readAll();
    auto dataRode = reader.dataRode();
    QStringList colNames{dataRode->header.getHeaderElements()};
    _processColNames(colNames);
    auto linesFormated = ColumnTree::instance()->getLinesFormated(
                colNames, dataRode->lines);
    LeadsTableModel::instance()->addLines(*linesFormated);
}

void PaneLeads::_importExcelFile(const QString &filePath)
{
    QXlsx::Document xlsx(filePath);

    // Get the used cell range.
    QXlsx::CellRange range = xlsx.dimension();
    if (!range.isValid()) {
        // No valid range was found (e.g. the file might be empty)
        return;
    }

    // Determine the boundaries of the used range.
    const int firstRow = range.firstRow();
    const int lastRow  = range.lastRow();
    const int firstCol = range.firstColumn();
    const int lastCol  = range.lastColumn();

    // Read the header row (assumed to be the first row of the range).
    QStringList colNames;
    for (int col = firstCol; col <= lastCol; ++col) {
        auto cell = xlsx.cellAt(firstRow, col);
        // If cell is null, we assume an empty header element.
        QString headerValue = cell ? cell->value().toString() : QString();
        colNames << headerValue;
    }

    // Process column names as done for CSV files.
    _processColNames(colNames);

    // Read the remaining rows as data lines.
    QList<QStringList> lines;
    for (int row = firstRow + 1; row <= lastRow; ++row) {
        QStringList rowData;
        for (int col = firstCol; col <= lastCol; ++col) {
            auto cell = xlsx.cellAt(row, col);
            QString cellValue = cell ? cell->value().toString() : QString();
            rowData << cellValue;
        }
        lines.append(rowData);
    }

    // Format the lines using ColumnTree and add them to the LeadsTableModel.
    auto linesFormated = ColumnTree::instance()->getLinesFormated(colNames, lines);
    LeadsTableModel::instance()->addLines(*linesFormated);
}

void PaneLeads::_processColNames(const QStringList &colNames)
{
    const QStringList &clearedColNames
            = ColumnTree::instance()->clearColumnsExisting(colNames);
    if (clearedColNames.size() > 0)
    {
        NewColumnsList columnsList{clearedColNames};
        DialogFileColumns dialog{&columnsList, this};
        dialog.exec();
    }
}

void PaneLeads::importLeadEmailsInteraction()
{
}

void PaneLeads::importLeadEmailsPurchase()
{
}

void PaneLeads::removeLeads()
{
    const auto &selIndexes
            = ui->tableViewCustomers->selectionModel()->selectedIndexes();
    if (selIndexes.size() > 0)
    {
        QSet<int> setRows;
        for (const auto &index : selIndexes)
        {
            setRows.insert(index.row());
        }
        QList<int> rows{setRows.begin(), setRows.end()};
        std::sort(rows.begin(), rows.end(), std::greater<>());
        for (const auto &row : rows)
        {
            LeadsTableModel::instance()->removeAt(row);
        }
    }
    else
    {
        QMessageBox::information(this,
                                 tr("No selection"),
                                 tr("You need to select leads to remove"));
    }
}


