#ifndef WPS_SET_DLG_H
#define WPS_SET_DLG_H

#include <QDialog>
#include "QDomDocument"
#include "QDate"
#include "SharedFiles/xml_namelist.h"
#include "QStatusBar"

namespace Ui {
    class WPS_set_dlg;
}

class WPS_set_dlg : public QDialog
{
    Q_OBJECT

public:
    /** Конструктор*/
    explicit WPS_set_dlg(QWidget *parent = 0);
    ~WPS_set_dlg();

private slots:
    /** Действия при завершении диалога по Ок*/
    void accept();
    /** Кнопка выбора папки GRIB*/
    void on_toolButton_browse_GRIB_clicked();
    /** Кнопка выбора выходной папки*/
    void on_toolButton_browse_out_clicked();
    /** Кнопка выбора wfs файла*/
    void on_toolButton_browse_wfs_clicked();
    /** Кнопка получения времени из GRIB файлов*/
    void on_pushButton_timeFromGRIB_clicked();

private:
    QStatusBar* statusBar; /**< указатель на статусбар*/
    xml_namelist* p_namelist;
    QDomDocument domDocument;
    void SetTimeDateForms();
    /** Читаем preferenes.xml*/
    void init();
    /** Читаем xml*/
    bool ReadPref(QString);
    /** Записываем preferenes.xml*/
    bool WritePref(QString);
    /** Записываем namelist.wfs*/
    bool WriteNamelistWFS();
    Ui::WPS_set_dlg *ui;
};

#endif // WPS_SET_DLG_H
