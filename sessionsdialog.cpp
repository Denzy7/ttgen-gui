#include "sessionsdialog.h"
#include "ui_sessionsdialog.h"
#include <QMessageBox>
#include <iostream>

sessionsdialog::sessionsdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sessionsdialog)
{
    ui->setupUi(this);
}

sessionsdialog::~sessionsdialog()
{
    delete ui;
}

void sessionsdialog::updateTable()
{
    //clear
    ui->tableWidget->setRowCount(0);
    std::vector<Session>::iterator it;
    for(it = sessions->begin(); it != sessions->end(); it++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        const int rows = ui->tableWidget->rowCount() - 1;
        //name
        ui->tableWidget->setItem(rows, 0, new QTableWidgetItem(it->name));

        //lec
        ui->tableWidget->setItem(rows, 1, new QTableWidgetItem(it->lecturer));

        //venue
        ui->tableWidget->setItem(rows, 2, new QTableWidgetItem(it->venue));
    }

}

void sessionsdialog::on_pushButton_clicked()
{
    if(ui->txtLecturer->text().length() <= 0 ||
            ui->txtName->text().length() <= 0 ||
            ui->txtVenue->text().length() <= 0)
    {
        QMessageBox::critical(this, "Error", "Cannot add. Not all fields are filled");
        return;
    }

    Session session;
    session.lecturer = ui->txtLecturer->text();
    session.name = ui->txtName->text();
    session.venue = ui->txtVenue->text();
    sessions->push_back(session);
    updateTable();
}

void sessionsdialog::on_pushButton_2_clicked()
{
    if(ui->tableWidget->currentRow() < 0)
    {
        QMessageBox::critical(this, "Error", "Cannot delete.No row selected");
    }
}


void sessionsdialog::on_pushButton_4_clicked()
{

}

