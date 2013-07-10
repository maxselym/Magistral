#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "wrffileread.h"
#include "xml_namelist.h"
#include <QCloseEvent>
#include "QTableWidget"


namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_LoadNetCDF_clicked();
    void on_LoadCSV_clicked();

    void on_Check_button_clicked();

private:
    void closeEvent(QCloseEvent * event);

private:
    void Analize();
    bool CSVReader(QString file_name);
    QTableWidget* Table;
    int FindItem(QString station_name);
    bool LoadPreferences(QString fileName);
    xml_namelist* station_info;
    WrfFileRead* wrffile;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
