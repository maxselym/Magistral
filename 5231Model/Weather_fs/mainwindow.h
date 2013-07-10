#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QWorkspace>
#include "special_param_dlg.h"
#include "raw_pgp_dlg.h"
#include "dlg_namelist_edit.h"
#include "wps_set_dlg.h"
#include "wpsrun_dlg.h"
#include "preference.h"

namespace Ui {
    class MainWindow;
}
/** Класс основного окна программы*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /** Конструктор*/
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
protected:
//    void changeEvent(QEvent *e);

private slots:
    /** Запуск дочернего окна настройки параметров WRF */
    void WPS_setup();
    /** Запуск дочернего окна настройки параметров namelist */
    void WPS_namelist();
    /** Запуск дочернего окна диспетчер запуска и выполнения*/
    void WPS_run();
    /** Запуск дочернего окна просмотра специальных параметров */
    void OpenSpecParam();
    /** Запуск дочернего окна просмотра данных от метеостанций*/
    void OpenRawPGP();
    void openWrf();
    void openWrfDA();
    /** Запуск модального диалогового окна настройки программы */
    void preferences();
    /** Запуск модального диалогового окна описания программы*/
    void about();

private:
    /** Указатель окна настройки параметров WRF */
    WPS_set_dlg* p_WPS_set_dlg;
    /** Указатель окна настройки параметров namelist */
    dlg_namelist_edit* p_namelist_wps;
    /** Указатель окна диспетчер запуска и выполнения*/
    wpsrun_dlg *p_wpsrun_dlg;
    /** Указатель окна просмотра специальных параметров */
    Special_param_dlg* p_special_param_dlg;
    /** Указатель окна данных от метеостанций */
    raw_pgp_dlg* p_raw_pgp_dlg;
    Ui::MainWindow *ui;
    /** Указатель на класс для работы с MDI */
    QWorkspace *workspace;

    /** Соединение всех слотов и сигналов*/
    void customize();
};

#endif // MAINWINDOW_H
