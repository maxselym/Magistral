#ifndef WPSRUN_H
#define WPSRUN_H

#include <QObject>
#include "QFile"
#include "QDir"
#include <QDate>
#include "xml_namelist.h"
#include "special_param.h"


const QString Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
/** Класс для подготовки и последовательного запуска файлов WRF*/
class WPSRun : public QObject
{
    Q_OBJECT
private:
    /** Путь к папке WRF*/
    QString wrf_root;
    /** Путь к папке WPS*/
    QString wps_root;
    /** Путь к папке WRFDA*/
    QString wrfda_root;
    /** Путь к папке ARWPost */
    QString arwpost_root;
    /** Путь к GRIB файлам*/
    QString grib_files;
    /** Путь к Geog файлам*/
    QString geog_files;
    /** Путь к namelist.wps*/
    QString namelist_wps;
    /** Путь к рабочей папке*/
    QString out_folder;
    /** Путь к рабочей папке*/
    QString work_folder;
    /** Номера доменов через запятую*/
    QString domain_to_export;
    /** Количество ядер*/
    int Core_num;
    /** Создать указатели типа GRIB.AAA*/
    void GribLink(QString sDir,QString work_folder);
    /** Указатель на класс по работе с namelist.wfs*/
    xml_namelist* p_namelist_tool;
    special_param* s_param;
public:
    /** Конструтор */
    explicit WPSRun(QObject *parent = 0);
    /** Копирование всех файлов  ввыходную папку*/
    /** Читаем preferenes.xml*/
    bool ReadPreferenceXML(QString);
    /** Читаем WFS и пишем конфиги*/
    bool ReadWFS();
    /** Выдаем ближайший трех часовой интервал**/
    int GetPrevSrok(int hours);
    bool initialize();
    /** Проверка GRIB файлов и считывание времени из них*/
    void SetTimeDateFromGRIB();

    void SetStartEndDateTime(QString start, QString end); //Формат "yyyy-MM-dd_hh:mm:ss"
    /** Установка начальной даты равной текущей*/
    void SetTimeDateStarttoCurrent();
    /** Очистить директорию рекурсивно*/
    void ClearDir(const QString d);
    QString GetTimeDateRUN();
    /** Рабочая папка*/
    QString GetOutputRoot();
    /** Получить Количество ядер*/
    int GetCoreNum();
    /** Путь к рабочей папке*/
    QString GetWorkFolder();
    /** Путь к выходной папке*/
    QString GetOutFolder();
    /** Перемещаем результаты расчета*/
    bool MoveWrfOut(QString s_dir, QString& report);
    /** Послать содержимое NetCDF файла в БД**/
    bool SendAlltoDB(QString &report);
    /** Сделать ссылку на файл результат **/
    QString GetWrfOutName(int n_domain);
    /** Сделать ссылку на файл результат **/
    QString GetOBSPROC3DVARName();
    /** Создать ссылку на wrfout_d0.... **/
    bool LinkWrfout(QString link_name);
//    QStringList GetWrfoutFiles();
signals:

public slots:

};

#endif // WPSRUN_H
