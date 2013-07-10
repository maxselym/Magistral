#include "special_param.h"
#include "QFile"
#include "qmath.h"


special_param::special_param(QObject *parent) :
    QObject(parent)
{
   // p_domDocument = new QDomDocument;
    db = new DBEngine;
    nc_file = NULL;
}

special_param::~special_param()
{
}

bool special_param::OpenNetCDFFile(QString fileName)
{
    // Open the file and check to make sure it's valid.
    nc_file = new NcFile(fileName.toAscii(), NcFile::ReadOnly);
    if(!nc_file->is_valid())
    {
        return false;
    }
    if(!db->IsConnected()) //Если нет соединения, то соединяем
    {
        if(!db->createConnection()) //Если нет соединение не открылось, то жалуемся
        {
            qDebug() << "Cannot open database:"<<db->LastError();
            return false;
        }
    }
    return true;
}

bool special_param::GetStationCoordinates(int st_name, float &x, float &y, float &z)
{
    return db->GetStationCoordinates(st_name,x,y,z);
}

void special_param::GetClosestXLAT_XLONG(int n_time, float f_in_lat, float f_in_long, int &out_lat, int &out_lon)
{
    //Берем размеры по осям
    int south_north = nc_file->get_dim(2)->size();
    int west_east = nc_file->get_dim(3)->size();

    //Объявляем массив, куда будем читать данные
    float var_xlat[west_east][south_north];
    float var_xlong[west_east][south_north];

    //Объявляем указатель и инициализируем переменной,
    //Название переменной смотрим через nc_dump -h <file>
    NcVar *var;
    if (!(var = nc_file->get_var("XLAT")))
       return;
    var->set_cur(n_time, 0, 0); //Передвигаемся в ячейку для чтения, нас интересует только время
    var->get(&var_xlat[0][0],1, west_east, south_north); //Вычитываем в Трех мерном массиве двумерный. Время = 1;

    if (!(var = nc_file->get_var("XLONG")))
       return;
    var->set_cur(n_time, 0, 0); //Передвигаемся в ячейку для чтения, нас интересует только время
    var->get(&var_xlong[0][0],1, west_east, south_north); //Вычитываем в Трех мерном массиве двумерный. Время = 1;

///Old algoritm
//    /** Вычисляем ближайший Х и ближайший Y**/
//    int nr_lat_xlat = 0;
//    int nr_lon_xlat = 0;
//    int nr_lat_xlong = 0;
//    int nr_lon_xlong = 0;

//    /** Разница между фактическим и модельным в первом узле**/
//    float f_temp_xlat = qAbs(var_xlat[0][0]-f_in_lat);
//    float f_temp_xlong = qAbs(var_xlong[0][0]-f_in_long);

//    /** Перебираем все узлы**/
//    for(int n_lat =0;n_lat<south_north; n_lat++)
//    {
//        for(int n_lon =0;n_lon<west_east; n_lon++)
//        {
//            if (qAbs(var_xlat[n_lat][n_lon]-f_in_lat)<f_temp_xlat) /** Если разница меньше, то запоминаем номер узла и обновляем разницу**/
//            {
//                f_temp_xlat = qAbs(var_xlat[n_lat][n_lon]-f_in_lat);

//                nr_lat_xlat = n_lat;
//                nr_lon_xlat = n_lon;
//            }
//            if(qAbs(var_xlong[n_lat][n_lon]-f_in_long)<f_temp_xlong) /** Если разница меньше, то запоминаем номер узла и обновляем разницу**/
//            {
//                f_temp_xlong = qAbs(var_xlong[n_lat][n_lon]-f_in_long);
//                nr_lat_xlong = n_lat;
//                nr_lon_xlong = n_lon;
//            }

//        }
//    }
//    /** Крест на крест, номер ближайшей ячейки**/
//    out_lat = nr_lat_xlat;
//    out_lon = nr_lon_xlong;
//________________________________
//New algoritm
    /** Вычисляем ближайший Х и ближайший Y**/
    out_lat = 0;
    out_lon = 0;


    /** Сумма разниц между фактическим и модельным в первом узле**/
    float distance = qAbs(var_xlat[0][0]-f_in_lat) + qAbs(var_xlong[0][0]-f_in_long);
    /** Перебираем все узлы**/
    for(int n_lat =0;n_lat<south_north; n_lat++)
    {
        for(int n_lon =0;n_lon<west_east; n_lon++)
        {
            if( (qAbs(var_xlat[n_lat][n_lon]-f_in_lat) + qAbs(var_xlong[n_lat][n_lon]-f_in_long)) < distance)    /** Если расстояние по Lat и Lon меньше то считаем ближайшим**/
            {
               distance = qAbs(var_xlat[n_lat][n_lon]-f_in_lat) + qAbs(var_xlong[n_lat][n_lon]-f_in_long);
               out_lat = n_lat;
               out_lon = n_lon;
            }

        }
    }
//    qDebug()<<"Input coords";
//    qDebug()<<f_in_lat;
//    qDebug()<<f_in_long;
//    qDebug()<<"Output coords";
//    qDebug()<<out_lat;
//    qDebug()<<out_lon;
//    qDebug()<<var_xlat[out_lat][out_lon];
//    qDebug()<<var_xlong[out_lat][out_lon];
}

/** Если b_max=true то береме максимум, иначе минимум
 * s_var XLAT или XLONG **/
float special_param::MinMaxXLAT_XLON(QString s_var, int n_time, bool b_max)
{
    //Берем размеры по осям
    int south_north = nc_file->get_dim(2)->size();
    int west_east = nc_file->get_dim(3)->size();
    //Объявляем массив, куда будем читать данные
    float var_in[south_north][west_east];
    //Объявляем указатель и инициализируем переменной,
    //Название переменной смотрим через nc_dump -h <file>
    NcVar *var;
    if (!(var = nc_file->get_var(s_var.toStdString().c_str())))
       return 0;
    var->set_cur(n_time, 0, 0); //Передвигаемся в ячейку для чтения, нас интересует только время
    var->get(&var_in[0][0],1, west_east, south_north); //Вычитываем в Трех мерном массиве двумерный. Время = 1;
    if(b_max)
        return var_in[south_north-1][west_east-1];
    else
        return var_in[0][0];

}

void special_param::FillSpecarr(int index, int n_time)
{
   // specarr->CreateTable(db->GetDB());
    QString st_num;
    st_num.setNum(index);
    float f_lat=0;
    float f_long=0;
    float z=0;
    GetStationCoordinates(index, f_lat,f_long,z);
    int out_lat = 0;
    int out_long = 0;
    GetClosestXLAT_XLONG(n_time,f_lat,f_long,out_lat,out_long);
    specarr = new specArr;
    specarr->pgp_index = st_num;
    specarr->date_time = QDateTime::fromString(GetTime(n_time),"yyyy-MM-dd_hh:mm:ss"); //Дата и время измерения
    specarr->rainc = GetAccumVar3D("RAINC", n_time, out_lat, out_long); //Дождь, кучевая облачность
    specarr->rainnc = GetAccumVar3D("RAINNC", n_time, out_lat, out_long); //Дождь
    specarr->hailnc = GetAccumVar3D("HAILNC", n_time, out_lat, out_long); //Град
    specarr->snownc = GetAccumVar3D("SNOWNC", n_time, out_lat, out_long); //Снег
    specarr->graupelnc = GetAccumVar3D("GRAUPELNC", n_time, out_lat, out_long); //Крупа
    specarr->snowh = GetVar3D("SNOWH", n_time, out_lat, out_long); // Высота снежного покрова
    specarr->psfc = GetPSFC(n_time, out_lat, out_long); //Приземное давление
    specarr->t2 = GetT2M_C(n_time, out_lat, out_long); //Температура воздуха
    specarr->windSpeed = GetWindSpeed(n_time,out_lat,out_long); //Скорость ветра
    specarr->windAngle = GetWindAngle(n_time,out_lat,out_long); //Направление ветра
    specarr->tRail = GetRailwayT(st_num, n_time,out_lat,out_long) - t_kelvin; //Температура рельса
    specarr->tslb = GetVar4DSoil("TSLB",n_time,1,out_lat,out_long) - t_kelvin; //Температура верхнего слоя почвы
    specarr->smois = GetVar4DSoil("SMOIS",n_time,1,out_lat,out_long); //Влажность верхнего слоя почвы
    specarr->glaccyIce = GetGlaccyIce(n_time,out_lat,out_long); //Гололедица
    specarr->hurricane = GetHurricane(n_time,out_lat,out_long); //Ураган
    specarr->InsertValues(db->GetDB());
}

bool special_param::writeCSVT2(QString st_num, QString f_name)
{
    QFile file(f_name);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << "Date,T2\n";
    int n_time = GetDimSize(0)-1;
    float x=0;
    float y=0;
    float z=0;
    GetStationCoordinates(st_num.toInt(), x,y,z);
    int out_x = 0;
    int out_y = 0;
    QString temp;
    while(n_time != 0)
    {
        GetClosestXLAT_XLONG(n_time,x,y,out_x,out_y);
        float t2 = GetT2M_C(n_time, out_x, out_y); //Температура воздуха
//        float t1 = t2-2.5;
//        float t3 = t2+ 2.5;
//        temp = QString("%1,%2,%3").arg(t1).arg(t2).arg(t3);
        temp.setNum(t2);
        QDateTime dt = QDateTime::fromString(GetTime(n_time),"yyyy-MM-dd_hh:mm:ss");
        out<<dt.toString("yyyy/MM/dd hh:mm") <<"," << temp <<"\n";
        n_time--;
    }
    file.close();
    return true;

}

bool special_param::writeCSVALLWind(int n_time, QString f_name)
{
    if(n_time<0 || n_time > GetDimSize(0)-1)
    {
        qDebug()<<"n_time out of range";
        return false;
    }

//    QDateTime dt = QDateTime::fromString(GetTime(n_time),"yyyy-MM-dd_hh:mm:ss");
//    out<<dt.toString("yyyy/MM/dd_hh:mm") <<"," << temp <<"\n";

    f_name = f_name+GetTime(n_time)+".csv";

    QFile file(f_name);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << "x, y, U, V, W, T2, T"<<"\n";
    int n_lev = 1;
    //Берем размеры по осям
    int south_north = nc_file->get_dim(2)->size();
    int west_east = nc_file->get_dim(3)->size();

    //Объявляем массив, куда будем читать данные
    float var_xlat[west_east][south_north];
    float var_xlong[west_east][south_north];

    //Объявляем указатель и инициализируем переменной,
    //Название переменной смотрим через nc_dump -h <file>
    NcVar *var;
    if (!(var = nc_file->get_var("XLAT")))
       return false;
    var->set_cur(n_time, 0, 0); //Передвигаемся в ячейку для чтения, нас интересует только время
    var->get(&var_xlat[0][0],1, west_east, south_north); //Вычитываем в Трех мерном массиве двумерный. Время = 1;

    if (!(var = nc_file->get_var("XLONG")))
       return false;
    var->set_cur(n_time, 0, 0); //Передвигаемся в ячейку для чтения, нас интересует только время
    var->get(&var_xlong[0][0],1, west_east, south_north); //Вычитываем в Трех мерном массиве двумерный. Время = 1;
    QString U;
    QString V;
    QString W;
    QString T2;
    QString T;

    for(int n_lat =0;n_lat<south_north; n_lat++)
    {
        for(int n_lon =0;n_lon<west_east; n_lon++)
        {
            U.setNum(GetVar4D("U",n_time,n_lev,n_lat,n_lon),'f',3);
            V.setNum(GetVar4D("V",n_time,n_lev,n_lat,n_lon),'f',3);
            W.setNum(GetVar4D("W",n_time,n_lev,n_lat,n_lon),'f',3);
            T.setNum(GetAtLevT_C(n_time,n_lev,n_lat,n_lon),'f',3);
            T2.setNum(GetT2M_C(n_time,n_lat,n_lon),'f',3);
            out<<var_xlat[n_lat][n_lon]<<","<<var_xlong[n_lat][n_lon]<<","
              <<U<<","<<V<<","<<W<<","<<T<<","<<T2<<"\n";
        }
    }
    file.close();
    return true;
}

QVector<int> special_param::GetStnListInArea()
{
    QVector<int> vector = db->GetStnIndexLst();
    float lat_max = MinMaxXLAT_XLON("XLAT",0,true);
    float lat_min = MinMaxXLAT_XLON("XLAT",0,false);
    float long_max = MinMaxXLAT_XLON("XLONG",0,true);
    float long_min = MinMaxXLAT_XLON("XLONG",0,false);
    for(int i=0;i<vector.count();i++)
    {
        float f_lat=0;
        float f_long=0;
        float z=0;
        if(GetStationCoordinates(vector[i], f_lat,f_long,z))
        {
            if( !(f_lat < lat_max && f_lat > lat_min && f_long < long_max && f_long > long_min) ) //Если станция не попадает в квадрат, то удаляем ее и начинаем с начала
            {
                vector.remove(i);
                i = 0;
            }
        }
    }
    return vector;
}

float special_param::GetVar4D(QString s_var, int n_time, int n_lev,int n_lat, int n_lon)
{
    //Берем размеры по осям
    int num_metgrid_levels = nc_file->get_dim(1)->size(); //Для wrf файла nc_file->get_dim(4)->size()
    int west_east = nc_file->get_dim(2)->size();
    int south_north = nc_file->get_dim(3)->size();

    //Проверяем на попадание в границы массива n_lat и n_lon
    if(n_lat>south_north || n_lat < 0)
    {
        qDebug()<<"4DVar error n_lat out of range: "<<n_lat;
        return 0;
    }
    if(n_lon>west_east || n_lon <0)
    {
        qDebug()<<"4DVar error n_lat out of range: "<<n_lon;
        return 0;
    }
    if(n_lev>num_metgrid_levels || n_lev <0)
    {
        qDebug()<<"4DVar error n_lev out of range: "<<n_lev;
        return 0;
    }
    //Объявляем массив, куда будем читать данные
    float var_in[num_metgrid_levels][south_north][west_east];
    //Объявляем указатель и инициализируем переменной,
    //Название переменной смотрим через nc_dump -h <file>

    NcError err(NcError::verbose_nonfatal); // Для того чтобы не вылетала сразу при отсутствии переменной в файле

    NcVar *var; //Указатель на переменную

    if(!(var = nc_file->get_var(s_var.toStdString().c_str())))   //Переводим QString в const char*  и запрашиваем переменную
    {
        /** http://www.unidata.ucar.edu/software/netcdf/docs/netcdf-c/Error-Codes.html#Error-Codes **/
        int n_err = err.get_err(); // Получаем код ошибки

        if(n_err == -49)
        {
            qDebug()<<s_var<<" Variable not found";
        } else
            qDebug()<<n_err;
        return 0;
    }



    //   return 0;
    // Read the data. Since we know the contents of the file we know
    // that the data arrays in this program are the correct size to
    // hold one timestep.
    var->set_cur(n_time, 0, 0, 0); //Передвигаемся в ячейку для чтения, нас интересует только время
    var->get(&var_in[0][0][0], 1, num_metgrid_levels, south_north, west_east); //Вычитываем в 4-х мерном массиве Трехмерный. Время = 1;
        //Пример вывода
    return var_in[n_lev][n_lat][n_lon];
}

float special_param::GetVar4DSoil(QString s_var, int n_time, int n_lev, int n_lat, int n_lon)
{
    //Берем размеры по осям
    int south_north = nc_file->get_dim(2)->size();
    int west_east = nc_file->get_dim(3)->size();
    int num_soil_levels = nc_file->get_dim(6)->size();

    //Проверяем на попадание в границы массива n_lat и n_lon
    if(n_lat>south_north || n_lat < 0)
    {
        qDebug()<<"4DVarSoil error n_lat out of range: "<<n_lat;
        return 0;
    }
    if(n_lon>west_east || n_lon <0)
    {
        qDebug()<<"4DVarSoil error n_lat out of range: "<<n_lon;
        return 0;
    }
    if(n_lev>num_soil_levels || n_lev <0)
    {
        qDebug()<<"4DVarSoil error n_lev out of range: "<<n_lev;
        return 0;
    }
    //Объявляем массив, куда будем читать данные
    float var_in[num_soil_levels][west_east][south_north];
    //Объявляем указатель и инициализируем переменной,
    //Название переменной смотрим через nc_dump -h <file>
    NcError err(NcError::verbose_nonfatal); // Для того чтобы не вылетала сразу при отсутствии переменной в файле

    NcVar *var; //Указатель на переменную

    if(!(var = nc_file->get_var(s_var.toStdString().c_str())))   //Переводим QString в const char*  и запрашиваем переменную
    {
        /** http://www.unidata.ucar.edu/software/netcdf/docs/netcdf-c/Error-Codes.html#Error-Codes **/
        int n_err = err.get_err(); // Получаем код ошибки

        if(n_err == -49)
        {
            qDebug()<<s_var<<" Variable not found";
        } else
            qDebug()<<n_err;
        return 0;
    }
    // Read the data. Since we know the contents of the file we know
    // that the data arrays in this program are the correct size to
    // hold one timestep.
    var->set_cur(n_time, 0, 0, 0); //Передвигаемся в ячейку для чтения, нас интересует только время
    var->get(&var_in[0][0][0], 1, num_soil_levels, west_east, south_north); //Вычитываем в 4-х мерном массиве Трехмерный. Время = 1;
        //Пример вывода
    return var_in[n_lev][n_lat][n_lon];
}

float special_param::GetVar3D(QString s_var, int n_time, int n_lat, int n_lon)
{
    //Берем размеры по осям
    int west_east = nc_file->get_dim(2)->size();
    int south_north = nc_file->get_dim(3)->size();

    //Проверяем на попадание в границы массива n_lat и n_lon
    if(n_lat>south_north || n_lat < 0)
    {
        qDebug()<<"3DVar error n_lat out of range: "<<n_lat;
        return 0;
    }
    if(n_lon>west_east || n_lon <0)
    {
        qDebug()<<"3DVar error n_lat out of range: "<<n_lon;
        return 0;
    }
    //Объявляем массив, куда будем читать данные
    float var_in[south_north][west_east];

    //Объявляем указатель и инициализируем переменной,
    //Название переменной смотрим через nc_dump -h <file>
    NcError err(NcError::verbose_nonfatal); // Для того чтобы не вылетала сразу при отсутствии переменной в файле

    NcVar *var; //Указатель на переменную

    if(!(var = nc_file->get_var(s_var.toStdString().c_str())))   //Переводим QString в const char*  и запрашиваем переменную
    {
        /** http://www.unidata.ucar.edu/software/netcdf/docs/netcdf-c/Error-Codes.html#Error-Codes **/
        int n_err = err.get_err(); // Получаем код ошибки

        if(n_err == -49)
        {
            qDebug()<<s_var<<" Variable not found";
        } else
            qDebug()<<n_err;
        return 0;
    }
    // Read the data. Since we know the contents of the file we know
    // that the data arrays in this program are the correct size to
    // hold one timestep.
    var->set_cur(n_time, 0, 0); //Передвигаемся в ячейку для чтения, нас интересует только время
    var->get(&var_in[0][0],1, south_north, west_east); //Вычитываем в Трех мерном массиве двумерный. Время = 1;
        //Пример вывода
    return var_in[n_lat][n_lon];
}

float special_param::GetVar1D(QString s_var, int n_time)
{
    //Объявляем массив, куда будем читать данные
    float var_in;
    //Объявляем указатель и инициализируем переменной,
    //Название переменной смотрим через nc_dump -h <file>
    NcError err(NcError::verbose_nonfatal); // Для того чтобы не вылетала сразу при отсутствии переменной в файле

    NcVar *var; //Указатель на переменную

    if(!(var = nc_file->get_var(s_var.toStdString().c_str())))   //Переводим QString в const char*  и запрашиваем переменную
    {
        /** http://www.unidata.ucar.edu/software/netcdf/docs/netcdf-c/Error-Codes.html#Error-Codes **/
        int n_err = err.get_err(); // Получаем код ошибки

        if(n_err == -49)
        {
            qDebug()<<s_var<<" Variable not found";
        } else
            qDebug()<<n_err;
        return 0;
    }
    // Read the data. Since we know the contents of the file we know
    // that the data arrays in this program are the correct size to
    // hold one timestep.
    var->set_cur(n_time, 0); //Передвигаемся в ячейку для чтения, нас интересует только время
    var->get(&var_in,1); //Вычитываем в Двумерном массиве одномерный. Время = 1;
        //Пример вывода
    return var_in;
}
int special_param::GetDimSize(int n)
{
    return nc_file->get_dim(n)->size();
}


/** Функция сравнения флоатов для q_sort()**/
int floatcomp(const void *elem1, const void *elem2)
{
    if(*(const float*)elem1 < *(const float*)elem2)
        return -1;
    return *(const float*)elem1 > *(const float*)elem2;
}

float special_param::GetVis(int n_time, int n_lat, int n_lon)
{
    float qrain = GetVar4D("QRAIN",n_time,0, n_lat,n_lon)*1000;
    float qsnow = GetVar4D("QSNOW",n_time,0, n_lat,n_lon)*1000;
    float qcloud = GetVar4D("QCLOUD",n_time,0, n_lat,n_lon)*1000;
    float ln0_02 = qLn(0.02);
    float array[3];
    array[0] = -ln0_02/(0.39+1.1*qPow(qrain,0.75));
    array[1] = -ln0_02/(0.39+10.4*qPow(qsnow,0.78));
    array[2] = -ln0_02/(0.39+144.7*qPow(qcloud,0.88));
    /** Библиотечная функция сорировки массива от меньшего к большему*/
    qsort(array, 3, sizeof(float), floatcomp);

    /** Возвращаем минимальное значение видимости*/
    return array[0];
}

float special_param::GetT2M_C(int n_time, int n_lat, int n_lon)
{
    return GetVar3D("T2",n_time,n_lat,n_lon) -t_kelvin;
}

float special_param::GetPSFC(int n_time, int n_lat, int n_lon)
{
    return GetVar3D("PSFC",n_time,n_lat,n_lon)/100;
}

float special_param::GetWindSpeed(int n_time, int n_lat, int n_lon)
{
    float V10 = GetVar3D("V10",n_time,n_lat,n_lon);
    float U10 = GetVar3D("U10",n_time,n_lat,n_lon);
    float V10ist = qSqrt(qPow(U10,2)+qPow(V10,2));
    return V10ist;

    //float z = 6; //Высота контактной сети в метрах
    //float ust = GetVar3D("UST",n_time,n_lat,n_lon);
    //float nn = V10ist-ust*qLn(10/z)/0.4;
    //Если величина маленька или отрицательная, тогда выдаем 0
//    if (nn < 0.1)
//        return 0;
//    else
//        return nn;
}

float special_param::GetWindAngle(int n_time, int n_lat, int n_lon)
{
    float V10 = GetVar3D("V10",n_time,n_lat,n_lon);
    float U10 = GetVar3D("U10",n_time,n_lat,n_lon);
  //  qDebug()<<4.0*atan (1.0)/180;

 //   qDebug()<<"U"<<U10;
  //  qDebug()<<"V"<<V10;
//    if(V10 > 0)
//    {
//        angle = (180 / pi) * atan(u/v) + 180;
//    } else if(U10 < 0 && V10 < 0)
//    {
//        angle = (180 / pi) * atan(u/v) + 0;
//    } else if(u > 0 & v < 0)
//    {
//        angle = (180 / pi) * atan(u/v) + 360;
//    }
    float DperR = 180/pi;//Для перевода радиан в градусы
  //  qDebug()<<"Atan2 check"<<atan2(1,-1)*DperR;



  //  float angle = (qAtan2(-U10,-V10)*DperR)+180; // Ветер куда дует

    float angle2 = (qAtan2(-U10,-V10)*DperR);
    if(angle2 <0)
        angle2 = angle2+360; //Ветер откуда дует

   // float angle3 = acos(U10/sqrt(U10*U10+V10*V10))*180/pi; //Противоположный истине
   // float angle4 = acos(V10/sqrt(U10*U10+V10*V10))*180/pi;
  //  qDebug()<< "-------------";
   // qDebug()<< angle;
   // qDebug()<< angle2;
    return angle2;
}

float special_param::GetRailwayT(QString st_num, int n_time, int n_lat, int n_lon)
{
    QString s_time = GetTime(n_time);
    QDateTime date = QDateTime::fromString(s_time,"yyyy-MM-dd_hh:mm:ss");

    date = date.addSecs(-60*60);

    float f_rail_t = db->GetRailT(st_num,date) + 273.16;
    if(f_rail_t < -999)
    {
        f_rail_t = GetVar4DSoil("TSLB",n_time,1,n_lat,n_lon);
    }
    float swdown = GetVar3D("SWDOWN",n_time,n_lat,n_lon);
    float glw = GetVar3D("GLW",n_time,n_lat,n_lon);
    float lh = GetVar3D("LH",n_time,n_lat,n_lon);
    float hfx = GetVar3D("HFX",n_time,n_lat,n_lon);
    float alf = 0.6; //Альбедо рельса
    float ps = 7700; //плотность стали
    float cs = 462; //Удельная теплоемкость стали
    float sigma = 5.670373/100000000; //Постоянная Стефана Больцмана
    float dt = 3600;
    float T = f_rail_t+dt*((swdown*(1-alf)+glw-sigma*qPow(f_rail_t,4)-lh-hfx)/(ps*cs));
    return T;
}

int special_param::GetGlaccyIce(int n_time, int n_lat, int n_lon)
{
    if((GetT2M_C(n_time,n_lat,n_lon)<0 && GetVar4D("QRAIN",n_time,0, n_lat,n_lon)>0) ||
            (GetT2M_C(n_time,n_lat,n_lon)>0 && GetVar4DSoil("TSLB",n_time,1,n_lat,n_lon)<t_kelvin))
        return 1;
    else return 0;
}

int special_param::GetHurricane(int n_time, int n_lat, int n_lon)
{
    if(n_time == 0)
        return 0;
    else
    {
        float delta = GetPSFC(n_time,n_lat,n_lon) - GetPSFC(n_time-1,n_lat,n_lon);
        if(delta > 2)
            return delta;
        else
            return 0;
    }
    return 0;
}

float special_param::GetAtLevPress(int n_time, int n_lev, int n_lat, int n_lon)
{
    return (GetVar4D("P",n_time,n_lev,n_lat,n_lon)+GetVar4D("PB",n_time,n_lev,n_lat,n_lon))*0.01;
}

float special_param::GetAtLevT_C(int n_time, int n_lev, int n_lat, int n_lon)
{
    float theta = t0+GetVar4D("T",n_time,n_lev,n_lat,n_lon); //Читаем потенциальную температуру для уровня, согласно документации t+300
    float p = GetVar4D("P",n_time,n_lev,n_lat,n_lon)+GetVar4D("PB",n_time,n_lev,n_lat,n_lon); //Читаем давление для уровня, согласно документации P+PB
    float base_press = GetVar1D("P00",n_time); //Читаем base pressure
    float tk = theta*qPow(p/base_press,kappa); //Вычисляем температуру для конкретного уровня
    return tk-t_kelvin;
}

float special_param::GetAtLevRH(int n_time, int n_lev, int n_lat, int n_lon)
{
    float t_c = GetAtLevT_C(n_time,n_lev,n_lat,n_lon);
    float p = GetVar4D("P",n_time,n_lev,n_lat,n_lon)+GetVar4D("PB",n_time,n_lev,n_lat,n_lon); //Читаем давление для уровня, согласно документации P+PB
    float x = GetVar4D("QVAPOR",n_time,n_lev,n_lat,n_lon);
    float q = x/(1.0+x);
    float es = es_alpha * qExp( es_beta * t_c /( t_c + es_gamma));
    float qs = rd_over_rv * es /( p - rd_over_rv1 * es);
    float rh = 100.0 * q / qs;
    return rh;
}

float special_param::GetAtLevTD_C(int n_time, int n_lev, int n_lat, int n_lon)
{
    float t = GetAtLevT_C(n_time,n_lev,n_lat,n_lon);
    float rh = GetAtLevRH(n_time,n_lev,n_lat,n_lon);
    float td = qPow(rh/100,0.125)*(112+0.9*t)+0.1*t-112;
    return td;
}

float special_param::GetAccumVar3D(QString s_var, int n_time, int n_lat, int n_lon)
{
    if(n_time == 0)
        return GetVar3D(s_var,0,n_lat,n_lon);
    else
    {
        return (GetVar3D(s_var,n_time,n_lat,n_lon) - GetVar3D(s_var,n_time-1,n_lat,n_lon));
    }
}

QString special_param::GetTime(int n)
{
    if(nc_file !=NULL)
    {
        int DateStrLen = nc_file->get_dim(1)->size(); //Получаем длину массива чаров, длину строки
        char* p_char = new char[DateStrLen]; //Объявляем массив чаров
        NcVar *var;
        if (!(var = nc_file->get_var("Times"))) //Берем двумерную переменную Times (массив строк)
            return 0;
        var->set_cur(n,0); //Идем в строку n
        var->get(p_char, 1,DateStrLen); //Берем одну строку состоящую из DateStrLen чаров
        QString str = p_char;
        str.truncate(DateStrLen);
        return str;
    } else return "";
}
