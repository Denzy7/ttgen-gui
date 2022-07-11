#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "sessionsdialog.h"
#include "breaksdialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<Session> sessions;
    std::vector<Break> breaks;

    std::list<Session> timetable;

    void updateSessionsLabel();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_btnGenerate_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
