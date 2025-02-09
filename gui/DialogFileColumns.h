#ifndef DIALOGFILECOLUMNS_H
#define DIALOGFILECOLUMNS_H

#include <QDialog>

class NewColumnsList;

namespace Ui {
class DialogFileColumns;
}

class DialogFileColumns : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFileColumns(
            NewColumnsList *newColumns, QWidget *parent = nullptr);
    ~DialogFileColumns();
    NewColumnsList *newColumnsListModel() const;

public slots:
    void addInTree();
    void removeFromTree();

private:
    static const QString TEXT_NO_SELECTION;
    Ui::DialogFileColumns *ui;
    void _connectSlots();
};

#endif // DIALOGFILECOLUMNS_H
