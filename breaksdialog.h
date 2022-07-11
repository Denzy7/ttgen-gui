#ifndef BREAKSDIALOG_H
#define BREAKSDIALOG_H

#include <QDialog>

typedef struct
{
    QString before_session;
    QString duration;
}Break;

namespace Ui {
class breaksdialog;
}

class breaksdialog : public QDialog
{
    Q_OBJECT

public:
    explicit breaksdialog(QWidget *parent = nullptr);
    ~breaksdialog();
    std::vector<Break>* breaks;
    void updateTable();

private slots:
    void on_pushButton_clicked();

private:
    Ui::breaksdialog *ui;
};

#endif // BREAKSDIALOG_H
