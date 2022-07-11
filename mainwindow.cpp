#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sessionsdialog.h"
#include "./ui_sessionsdialog.h"
#include <cstring>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QFileDialog>

#define arysz(ary) sizeof(ary) / sizeof(ary[0])
#define breakstr "BREAK"
#define breaktok ";"
#define tmfmt "%H:%M"

void shuffle(int* array, int n);

void shuffle(int* array, int n)
{
    // very tame clock precision. at least its cross platform...
    unsigned int seed = (unsigned int)((float)((time(NULL) / clock()) * 13.15f));
    srand(seed);
    for(int i = 0; i < n; i++)
    {
        int current = array[i]; // current value
        int to = rand() % n; // where to go

        array[i] = array[to]; // i = to
        array[to] = current; // to = current
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    static const Session sample_sessions[]=
    {
        //name, venue, lec
        {"Calculus", "EG-28", "Dr. Jane"},
        {"Application Programming", "Lab-A04", "Mr. Mwangi"},
        {"Operation Research", "EG-32", "Dr. Dishon"},
        {"Computer Graphics", "Lab-B07", "Mrs. Beatrice"},
        {"Principles of Marketing", "EG-30", "Dr. Temoi"},
        {"x86 Assembly Programming", "Lab-E02", "Dr. Carol"},
    };

    static const Break sample_breaks[]=
    {
        //b4_session, duration(mins)
        {"2", "30"},
        {"4", "45"}
    };

    for(int i = 0; i < arysz(sample_sessions); i++)
        sessions.push_back(sample_sessions[i]);

    for(int i = 0; i < arysz(sample_breaks); i++)
        breaks.push_back(sample_breaks[i]);

    updateSessionsLabel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSessionsLabel()
{
    char sessionscount[256];
    snprintf(sessionscount, sizeof(sessionscount), "%lu", sessions.size());
    ui->lblSessionsCount->setText(sessionscount);
}

void MainWindow::on_pushButton_clicked()
{
    sessionsdialog sess(this);
    sess.sessions = &sessions;
    sess.updateTable();
    sess.exec();
    updateSessionsLabel();
}


void MainWindow::on_pushButton_2_clicked()
{
    breaksdialog brks(this);
    brks.breaks = &breaks;
    brks.updateTable();
    brks.exec();
}

#include <iostream>
#include <forward_list>
void MainWindow::on_btnGenerate_clicked()
{
    time_t tStart = (ui->tedSessionsStart->time().hour() * 3600) + (ui->tedSessionsStart->time().minute() * 60);
    time_t tSessionDuration = (ui->tedSessionDuration->time().hour() * 3600) + (ui->tedSessionDuration->time().minute() * 60);
    int nColumns = ui->txtDailySessions->text().toInt();
    //assume 5 day week
    int nRows = 5;
    ui->tableWidget->setColumnCount(nColumns);
    const int nElements = nColumns * nRows;

    //pool some ints with values of session id's and shuffle them
    int* pool = new int[nElements];
    int session_id = 0;
    for(int i = 0; i < nElements; i++)
    {
        pool[i] = session_id;
        session_id++;
        if(session_id > sessions.size() - 1)
            session_id = 0;
    }
    shuffle(pool, nElements);

    if(ui->cbxFillMode->currentIndex() == 1)
    {
        //remove repeated sessions i.e = -1
        for(int i = 0; i < nElements; i++)
        {
            for(int j = i + 1; j < nElements; j++)
            {
                if(pool[i] == pool[j] && pool[j] != -1)
                    pool[j] = -1;
            }
        }
    }

    timetable.clear();

    for(uint32_t i = 0; i < nElements; i++)
    {
        Session s;
        if(pool[i] == -1)
        {
            //blank session (for fill once)
            s.lecturer = "";
            s.name = "";
            s.venue = "";
        }else
        {
            s = sessions.at(pool[i]);
        }
        timetable.push_back(s);
    }

    std::list<Session>::iterator it;
    int iColumn = 0, iRow = 0;
    int nAddedBreaks = 0;
    for(it = timetable.begin(); it != timetable.end(); it++)
    {
        //insert breaks
        for(uint32_t i = 0; i < breaks.size(); i++)
        {
            Break brk = breaks.at(i);

            if(brk.before_session.toInt() == iColumn + 1)
            {
                Session brksess;
                QString brkstr_duration = QString(breakstr) + breaktok + brk.duration;
                brksess.name = brkstr_duration;
                brksess.lecturer = breakstr;
                brksess.venue = breakstr;
                timetable.insert(it, brksess);
                nAddedBreaks++;
            }
        }

        //std::cout<<iColumn<<":"<<iRow<<std::endl;
        iColumn++;
        if(iColumn >= nColumns){
            iColumn = 0;
            if(iRow < nRows)
                iRow++;
        }
    }

    //account for added break columns per row
    nColumns += (nAddedBreaks / nRows);

    ui->tableWidget->setRowCount(nRows);
    ui->tableWidget->setColumnCount(nColumns);

    //iterate once more to do the final table fill
    iColumn = 0, iRow = 0;
    char tableItemStr[256];
    time_t tEnded = tStart;
    struct tm* t;
    for(it = timetable.begin(); it != timetable.end(); it++)
    {
        const std::string lec = it->lecturer.toStdString();
        const std::string venue = it->venue.toStdString();
        const std::string name = it->name.toStdString();

        char tmBf1[32], tmBf2[32];
        t = gmtime(&tEnded);
        strftime(tmBf1, sizeof(tmBf1), tmfmt, t);

        //preety print break
        if(strcmp(venue.c_str(), breakstr) == 0)
        {
            snprintf(tableItemStr, sizeof(tableItemStr),
                     "%c", breakstr[iRow]);
            QStringList list = it->name.split(breaktok);
            //convert to time_t
            tEnded+=(list[1].toUInt() * 60);
        }else
        {

            snprintf(tableItemStr, sizeof(tableItemStr),
                     "%s\n %s %c %s",
                     name.c_str(),
                     venue.c_str(),
                     name != "" ? ':' : ' ', /* evil hack when filling once */
                     lec.c_str());
            tEnded+=tSessionDuration;
        }

        //std::cout<< tableItemStr << std::endl;
        ui->tableWidget->setItem(iRow, iColumn, new QTableWidgetItem(tableItemStr));

        t = gmtime(&tEnded);
        strftime(tmBf2, sizeof(tmBf2), tmfmt, t);
        snprintf(tableItemStr, sizeof(tableItemStr), "%s - %s", tmBf1, tmBf2);
        ui->tableWidget->setHorizontalHeaderItem(iColumn, new QTableWidgetItem(tableItemStr));
//        std::cout<<tableItemStr<<std::endl;

        iColumn++;
        if(iColumn >= nColumns){
            iColumn = 0;
            if(iRow < nRows){
                iRow++;
                //restart the day
                tEnded = tStart;
            }
        }
    }

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    delete[] pool;
}

void MainWindow::on_pushButton_3_clicked()
{
    QString save = QFileDialog::getSaveFileName(this, "Select where to save .pdf");
    QPixmap pixmap;
    pixmap = ui->tableWidget->grab();

    QPrinter printer;
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFormat(QPrinter::PdfFormat);

    printer.setOutputFileName(save);

    QPainter painter(&printer);
    QRect rect = painter.viewport();
    QSize size = pixmap.size();

    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(pixmap.rect());

    painter.drawPixmap(0,0, size.width(), size.height(), pixmap);
}

