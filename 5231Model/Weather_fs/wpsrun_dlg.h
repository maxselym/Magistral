#ifndef WPSRUN_DLG_H
#define WPSRUN_DLG_H

#include <QDialog>
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

namespace Ui {
    class wpsrun_dlg;
}
/** Класс-форма для подготовки и последовательного запуска файлов WRF*/
class wpsrun_dlg : public QDialog
{
    Q_OBJECT

public:
    /** Конструктор*/
    explicit wpsrun_dlg(QWidget *parent = 0);
    ~wpsrun_dlg();

    void ProcSetup();
private slots:
    /** Кнопка запуска счета*/
    void on_pushButton_run_clicked();
    /** Последовательный запуск процессов*/
    void Run_proc(QString output_root, int n);
    /** Прочесть весь вывод процесса*/
    void ReadStdout();
    /** Действия при завершении работы процесса*/
    void Finished(int n,QProcess::ExitStatus);
    /** Задать параметры времени по GRIB файлам*/
    void on_pushButton_time_from_grib_clicked();
    void on_pushButton_init_clicked();

    void on_pushButton_geogrid_clicked();

    void on_pushButton_Ungrib_clicked();

    void on_pushButton_Metgrid_clicked();

    void on_pushButton_Real_clicked();

    void on_pushButton_da_clicked();

    void on_pushButton_UpdateBC_clicked();

    void on_pushButton_OBSPROC_clicked();

    void on_pushButton_wrf_clicked();

private:
    bool b_manual;
    /** Отключить элементы управления*/
    void DisableControls(bool b);
    /** Указатель на Класс для подготовки и последовательного запуска файлов WRF*/
    WPSRun* p_wpsrun;
    /** Номер процесса*/
    int n_proc;
    /** Указатель на процесс*/
    QProcess* proc;
    Ui::wpsrun_dlg *ui;
};

#endif // WPSRUN_DLG_H
