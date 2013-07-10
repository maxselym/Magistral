#ifndef SPECIAL_PARAM_H
#define SPECIAL_PARAM_H

#include <QObject>
#include "netcdfcpp.h"
#include "QDebug"
#include "QDomDocument"
#include "QStringList"
#include "SharedFiles/specarr.h"
#include "SharedFiles/dbengine.h"

/** Фундаментальные константы*/
const float pi = 3.1415926;   //Число пи
const float t0 = 300; //
const float t_kelvin = 273.15; //0 кельвинах от
const float gas_constant = 287.0; //Универса́льная га́зовая постоя́нная
const float gas_constant_v = 461.6;
const float cp = 7.0*gas_constant/2.0;
const float kappa = gas_constant / cp;

const float rd_over_rv = gas_constant / gas_constant_v;
const float rd_over_rv1 = 1.0 - rd_over_rv;
const float L_over_Rv = 5418.12;
//Газовые константы
//Saturation Vapour Pressure Constants(Rogers & Yau, 1989)
const float es_alpha = 611.2;
const float es_beta = 17.67;
const float es_gamma = 243.5;
const float es_gammabeta = es_gamma * es_beta;
const float es_gammakelvin = es_gamma - t_kelvin;

/** Класс по расчету специальных параметров*/
class special_param : public QObject
{
    Q_OBJECT
public:
    /** Конструктор*/
    explicit special_param(QObject *parent = 0);
    ~special_param();
    /** Открыть NetCDF файл*/
    bool OpenNetCDFFile(QString fileName);
    /** Открыть XML c Х и Y станций*/
    bool LoadStationFile();
    /** Указатель на класс по работе с таблцей**/
    DBEngine* db;
    /** Получить список станций из p_domDocument*/
    QVector<int> GetStnListInArea();
    /** Получить размер оси*/
    int GetDimSize(int n);
    /** Приземные параметры*/
    float GetVis(int n_time, int n_lat, int n_lon); /**< Расчет видимости*/
    float GetT2M_C(int n_time, int n_lat, int n_lon); /**< Температура в Цельсиях*/
    float GetPSFC(int n_time, int n_lat, int n_lon); /**<  Давление в гПа*/
    float GetWindSpeed(int n_time, int n_lat, int n_lon); /** Скорость ветра на высоте контактной сети*/
    float GetWindAngle(int n_time,int n_lat,int n_lon); /**< Направление ветра*/
    float GetRailwayT(QString st_num, int n_time, int n_lat, int n_lon); /** Температура рельса*/
    int GetGlaccyIce(int n_time, int n_lat, int n_lon); /** Гололедица */
    int GetHurricane(int n_time, int n_lat, int n_lon); /** Ураган */
    /** Параметры по уровням*/
    float GetAtLevPress(int n_time, int n_lev,int n_lat, int n_lon); /**< Давление в гПа*/
    float GetAtLevT_C(int n_time, int n_lev,int n_lat, int n_lon); /**< Температура в Цельсиях*/
    float GetAtLevRH(int n_time, int n_lev,int n_lat, int n_lon);  /**< Относительная влажность %*/
    float GetAtLevTD_C(int n_time, int n_lev,int n_lat, int n_lon); /**< Точка росы в Цельсиях*/
    float GetAccumVar3D(QString s_var, int n_time, int n_lat, int n_lon); /** Трехмерная переменная c вычетом накопления */

    QString GetTime(int n); /** Время виде строки 2012-03-16_03:00:00*/
    float GetVar1D(QString s_var, int n_time); /** Одномерная переменная*/
    float GetVar3D(QString s_var, int n_time, int n_lat, int n_lon); /** Трехмерная переменная*/
    float GetVar4D(QString s_var, int n_time, int n_lev,int n_lat, int n_lon); /** Четырехмерная переменная для воздуха*/
    float GetVar4DSoil(QString s_var, int n_time, int n_lev,int n_lat, int n_lon);  /** Четырехмерная переменная для почв*/
    bool GetStationCoordinates(int st_name, float& x, float& y, float& z);  /** Получить номер квадрата XYZ по имени станции из XML*/
    void GetClosestXLAT_XLONG(int n_time, float f_in_lat, float f_in_long, int &out_lat, int &out_lon); /** Получить ближайший квадрат к выбранным координатам*/
    float MinMaxXLAT_XLON(QString s_var, int n_time, bool b_max);

    void FillSpecarr(int index, int n_time);

    //Вывод температуры для графиков Ильи, больше не требуется.
    bool writeCSVT2(QString st_num, QString f_name);
    bool writeCSVALLWind(int n_time, QString f_name);
private:
    NcFile* nc_file; /** Указатель на NetCDF файл*/
    specArr* specarr; /** Указатель на массив для хранения спец параметров*/

signals:

public slots:

};

#endif // SPECIAL_PARAM_H
