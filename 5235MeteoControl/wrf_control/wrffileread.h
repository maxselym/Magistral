#ifndef WRFFILEREAD_H
#define WRFFILEREAD_H

#include <QObject>
#include "netcdfcpp.h"

class WrfFileRead : public QObject
{
    Q_OBJECT
public:
    explicit WrfFileRead(QObject *parent = 0);
    bool OpenFile(QString fileName);
    bool GetInfo();
    float GetVar3D(QString s_var, int n_time, int n_lat, int n_lon);
    float GetVar4D(QString s_var, int n_time, int n_lev,int n_lat, int n_lon);
private:
    NcFile* nc_file;
    NcDim* dimTime; //время
    NcDim* dimlat;//долгота
    NcDim* dimlon;//широта
    NcDim* dimMetLev;//Метео уровень
public slots:

};

#endif // WRFFILEREAD_H
