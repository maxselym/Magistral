#ifndef WRFFILEREAD_H
#define WRFFILEREAD_H

#include <QObject>
#include "netcdfcpp.h"

class WrfFileRead : public QObject
{
    Q_OBJECT
public:
    int south_north;
    int west_east;
    int num_metgrid_levels;
    explicit WrfFileRead(QObject *parent = 0);
    bool OpenFile(QString fileName);
    bool GetInfo();
    float GetPressure(int n_lev,int n_lat, int n_lon);
    float GetTemperature(int n_lev,int n_lat, int n_lon);
private:
    NcFile* nc_file;
    NcDim* dimTime; //время
    NcDim* dimlat;//долгота
    NcDim* dimlon;//широта
    NcDim* dimMetLev;//Метео уровень
public slots:

};

#endif // WRFFILEREAD_H
