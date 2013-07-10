#include "wrffileread.h"
#include "QDebug"

WrfFileRead::WrfFileRead(QObject *parent) :
    QObject(parent)
{
    nc_file = 0;
}

bool WrfFileRead::OpenFile(QString fileName)
{
    // Open the file and check to make sure it's valid.
    nc_file = new NcFile(fileName.toAscii(), NcFile::ReadOnly);
    if(!nc_file->is_valid())
       return false;
    else
    {
        qDebug()<<"File opened!";
        return true;
    }
}

bool WrfFileRead::GetInfo()
{
    if(nc_file == 0)
        return false;
    qDebug()<<QString("number of dimensions in the netCDF file: %1").arg(nc_file->num_dims());
    qDebug()<<QString("number of variables in the netCDF file: %1").arg(nc_file->num_vars());
    qDebug()<<QString("number of global attribute in the netCDF file: %1").arg(nc_file->num_atts());
    qDebug()<<"All dimentions:";
    for(int i =0;i<nc_file->num_dims();i++)
    {
          qDebug()<<QString("Dimention number %1 is: %2").arg(i).arg(nc_file->get_dim(i)->size());
    }
    return true;
}

float WrfFileRead::GetVar4D(QString s_var, int n_time, int n_lev,int n_lat, int n_lon)
{
    //Берем размеры по осям
    int south_north = nc_file->get_dim(2)->size();
    int west_east = nc_file->get_dim(3)->size();
    int num_metgrid_levels = nc_file->get_dim(4)->size();

    //Объявляем массив, куда будем читать данные
    float var_in[num_metgrid_levels][west_east][south_north];


    //Объявляем указатель и инициализируем переменной,
    //Название переменной смотрим через nc_dump -h <file>
    NcVar *var;
    const char* myChar = s_var.toStdString().c_str(); //Переводим QString в const char*
    if (!(var = nc_file->get_var(myChar)))
       return 0;
    // Read the data. Since we know the contents of the file we know
    // that the data arrays in this program are the correct size to
    // hold one timestep.
    var->set_cur(n_time, 0, 0, 0); //Передвигаемся в ячейку для чтения, нас интересует только время
    var->get(&var_in[0][0][0], 1, num_metgrid_levels, west_east, south_north); //Вычитываем в 4-х мерном массиве Трехмерный. Время = 1;
        //Пример вывода
    return var_in[n_lev][n_lat][n_lon];
}

float WrfFileRead::GetVar3D(QString s_var, int n_time, int n_lat, int n_lon)
{
    //Берем размеры по осям
    int south_north = nc_file->get_dim(2)->size();
    int west_east = nc_file->get_dim(3)->size();


    //Объявляем массив, куда будем читать данные
    float var_in[west_east][south_north];


    //Объявляем указатель и инициализируем переменной,
    //Название переменной смотрим через nc_dump -h <file>
    NcVar *var;
    const char* myChar = s_var.toStdString().c_str(); //Переводим QString в const char*
    if (!(var = nc_file->get_var(myChar)))
       return 0;
    // Read the data. Since we know the contents of the file we know
    // that the data arrays in this program are the correct size to
    // hold one timestep.
    var->set_cur(n_time, 0, 0); //Передвигаемся в ячейку для чтения, нас интересует только время
    var->get(&var_in[0][0],1, west_east, south_north); //Вычитываем в Трех мерном массиве двумерный. Время = 1;
        //Пример вывода
    return var_in[n_lat][n_lon];
}
