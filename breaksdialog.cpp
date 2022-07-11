#include <QMessageBox>

#include "breaksdialog.h"
#include "ui_breaksdialog.h"

breaksdialog::breaksdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::breaksdialog)
{
    ui->setupUi(this);
}

void breaksdialog::updateTable()
{
    //clear
    ui->tableWidget->setRowCount(0);
    std::vector<Break>::iterator it;
    for(it = breaks->begin(); it != breaks->end(); it++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        const int rows = ui->tableWidget->rowCount() - 1;
        //after sess
        ui->tableWidget->setItem(rows, 0, new QTableWidgetItem(it->before_session));

        //duration
        ui->tableWidget->setItem(rows, 1, new QTableWidgetItem(it->duration));
    }
}

breaksdialog::~breaksdialog()
{
    delete ui;
}

void breaksdialog::on_pushButton_clicked()
{
    if(ui->txtDuration->text().length() <= 0 ||
            ui->txtAfterSession->text().length() <= 0)
    {
        QMessageBox::critical(this, "Error", "Cannot add. Not all fields are filled");
        return;
    }

    Break brk;
    brk.before_session = ui->txtAfterSession->text();
    brk.duration = ui->txtDuration->text();
    breaks->push_back(brk);
    updateTable();
}

