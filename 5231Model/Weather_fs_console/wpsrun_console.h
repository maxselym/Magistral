#ifndef WPSRUN_CONSOLE_H
#define WPSRUN_CONSOLE_H

#include <QObject>
#include <QProcess>
#include "SharedFiles/wpsrun.h"

enum
{
    GEOGRID,
    UNGRIB,
    METGRID,
    REAL,
    OBSPROC,
    DA,
    UPDATEBC,
    WRF,
    ARWpost
};
class wpsrun_console : public QObject
{
    Q_OBJECT
public:
    explicit wpsrun_console(QObject *parent = 0);
    bool Init();
    void Start();
private slots:
    /** Последовательный запуск процессов*/
    void Run_proc(QString output_root, int n);
    /** Прочесть весь вывод процесса*/
    void ReadStdout();
    /** Действия при завершении работы процесса*/
    void Finished(int n,QProcess::ExitStatus);
    /** Задать параметры времени по GRIB файлам*/
    void TimeFromGRIB();
private:

    /** Указатель на Класс для подготовки и последовательного запуска файлов WRF*/
    WPSRun* p_wpsrun;
    /** Номер процесса*/
    int n_proc;
    /** Указатель на процесс*/
    QProcess* proc;
signals:
    void allDone(int); //we add a new signal
public slots:
    
};

#endif // WPSRUN_CONSOLE_H
