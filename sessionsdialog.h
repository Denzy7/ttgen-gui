#ifndef SESSIONSDIALOG_H
#define SESSIONSDIALOG_H

#include <QDialog>

typedef struct
{
    QString name;
    QString venue;
    QString lecturer;
}Session;

namespace Ui {
class sessionsdialog;
}

class sessionsdialog : public QDialog
{
    Q_OBJECT

public:
    explicit sessionsdialog(QWidget *parent = nullptr);
    ~sessionsdialog();
    std::vector<Session>* sessions;

    void updateTable();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::sessionsdialog *ui;
};

#endif // SESSIONSDIALOG_H
