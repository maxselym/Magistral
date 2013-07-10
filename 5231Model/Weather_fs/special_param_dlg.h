#ifndef SPECIAL_PARAM_DLG_H
#define SPECIAL_PARAM_DLG_H

#include <QDialog>
#include "QTableWidget"
#include "QStatusBar"
#include "SharedFiles/special_param.h"
#include "SharedFiles/little_r_fm12.h"
namespace Ui {
    class Special_param_dlg;
}
/** Класс-форма по расчету специальных параметров*/
class Special_param_dlg : public QDialog
{
    Q_OBJECT

public:
    /** Конструктор*/
    explicit Special_param_dlg(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    ~Special_param_dlg();

private slots:
    /** Нажатие на кнопку LoadNetCDF*/
    void on_pushButton_loadNetCDF_clicked();
    /** При выборе станции в списке*/
    void on_tableWidget_itemSelectionChanged();
    /** При передвижении слайдера*/
    void on_horizontalSlider_Time_valueChanged(int value);

    void on_pushButtonSendAlltoDB_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_pushButtonSendSelectedToDB_clicked();

    void on_pushButtonSendSelectedLittle_r_ASCII_clicked();

    void on_pushButtonSendAllLittle_R_clicked();

private:
    /** Заполнить список станций*/
    bool FetchStationList();
    /** Поменять заголовок в соответствии с открытым файлом*/
    void SetTitle(QString s_title);
    Ui::Special_param_dlg *ui;
    QStatusBar* statusBar; /**< указатель на статусбар*/
    special_param* s_param; /**< указатель на Класс по расчету специальных параметров*/


    QProcess* proc;
};

#endif // SPECIAL_PARAM_DLG_H
