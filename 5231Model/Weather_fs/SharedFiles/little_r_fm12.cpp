#include "little_r_fm12.h"

Little_r_fm12::Little_r_fm12(QObject *parent) :
    QObject(parent)
{
    p_file = NULL;
    p_stream = NULL;
}

bool Little_r_fm12::OpenFile(QString s_file)
{
    p_file = new QFile(s_file);
    p_file->open(QIODevice::WriteOnly | QIODevice::Text);
    p_stream = new QTextStream(p_file);
    return true;
}

bool Little_r_fm12::CloseFile()
{
    p_stream->flush();
    p_file->close();
    return true;
}

bool Little_r_fm12::writeRZD_DB_data(int index, QDateTime date)
{
    float f_t_air;
    float f_press;
    float f_wind;
    float f_wind_dir;
    float f_rh;
    DBEngine* db;
    db = new DBEngine(this);
    if(!db->createConnection()) //Если нет соединение не открылось, то жалуемся
    {
        qDebug() << "Cannot open database:"<<db->LastError();
        return false;
    }
    db->GetStnInfoDyIndexAndTime(date, index, f_t_air, f_press, f_wind, f_wind_dir, f_rh);
    f_press = f_press*100; //Переводим в Паскали
    f_t_air = f_t_air + 273.15; //Переводим в Кельвины;

    float f_lat;
    float f_lon;
    float z;
    QString s_index;
    s_index.setNum(index);
    db->GetStationCoordinates(index,f_lat,f_lon,z);
    db->closeConnection();
    writeHeader(f_lat,f_lon,s_index,"RZD obs","FM-12 TEMP","",z,5,0,0,1,0,true,false,false,NoData,NoData,date,f_press,0);
    writeData(f_press,z,f_t_air,NoData,f_wind,f_wind_dir,NoData,NoData,f_rh,NoData);
    writeEnding();
    writeTail(1,0,0); //Одно измерение
    return true;
}

bool Little_r_fm12::writeHeader(float f_lat, float f_lon, //Координаты
                                QString ID, // ID станции (цифры)
                                QString Name, //Имя станции, например (SFC OBS from NCAR ADP DS464.0)
                                QString Platform, //Платформа (FM-12 TEMP)
                                QString Source, //ХЗ
                                float f_elevation, //Высота станции
                                int num_vld__fld,  //Количество полей
                                int num_error,     //Количество ошибок
                                int num_warrning,  //Количество предупреждений
                                int Seq_num,       //Количество измерений
                                int num_dupd,      //ХЗ
                                bool is_sound,     //Атмосферное зондирование
                                bool bogus,
                                bool discard,
                                int v_time_sut, int v_time_jul, QDateTime date,
                                float slp, int slp_qc)
{
    QString xStr;
    xStr = QString("%1").arg(f_lat, 20, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(f_lon, 20, 'f', 5);
    *p_stream<<xStr;
    xStr = ID;
    while(ID.length()<40)
    {
        ID = ID + " ";
    }
    *p_stream<<ID;
    while(Name.length()<40)
    {
        Name = Name + " ";
    }
    *p_stream<<Name;
    while(Platform.length()<40)
    {
        Platform = Platform + " ";
    }
    *p_stream<<Platform;
    while(Source.length()<40)
    {
        Source = Source + " ";
    }
    *p_stream<<Source;
    xStr = QString("%1").arg(f_elevation, 20, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(num_vld__fld, 10);
    *p_stream<<xStr;
    xStr = QString("%1").arg(num_error, 10);
    *p_stream<<xStr;
    xStr = QString("%1").arg(num_warrning, 10);
    *p_stream<<xStr;
    xStr = QString("%1").arg(Seq_num, 10);
    *p_stream<<xStr;
    xStr = QString("%1").arg(num_dupd, 10);
    *p_stream<<xStr;
    if(is_sound)
        *p_stream<<"         T";
    else
        *p_stream<<"         F";
    if(bogus)
        *p_stream<<"         T";
    else
        *p_stream<<"         F";
    if(discard)
        *p_stream<<"         T";
    else
        *p_stream<<"         F";
    xStr = QString("%1").arg(v_time_sut, 10);
    *p_stream<<xStr;
    xStr = QString("%1").arg(v_time_jul, 10);
    *p_stream<<xStr;
    QString dateStr = date.toString("yyyyMMddhhmmss");
    while(dateStr.length()<20)
    {
        dateStr = " "+dateStr;
    }
    *p_stream<<dateStr;
    xStr = QString("%1").arg(slp, 13, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(slp_qc, 7);
    *p_stream<<xStr;
    for(int i=0;i<12;i++)
    {
        xStr = QString("%1").arg(-888888., 13, 'f', 5);
        *p_stream<<xStr;
        xStr = QString("%1").arg(0, 7);
        *p_stream<<xStr;
    }
    *p_stream<<"\n";
    return true;
}

bool Little_r_fm12::writeData(float press, float height, float temp,
                              float dew_point, float w_speed, float w_dir,
                              float U, float V, float RH, float Thicknes)
{
    QString xStr;
    xStr = QString("%1").arg(press, 13, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(0, 7);
    *p_stream<<xStr;
    xStr = QString("%1").arg(height, 13, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(0, 7);
    *p_stream<<xStr;
    xStr = QString("%1").arg(temp, 13, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(0, 7);
    *p_stream<<xStr;
    xStr = QString("%1").arg(dew_point, 13, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(0, 7);
    *p_stream<<xStr;
    xStr = QString("%1").arg(w_speed, 13, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(0, 7);
    *p_stream<<xStr;
    xStr = QString("%1").arg(w_dir, 13, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(0, 7);
    *p_stream<<xStr;
    xStr = QString("%1").arg(U, 13, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(0, 7);
    *p_stream<<xStr;
    xStr = QString("%1").arg(V, 13, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(0, 7);
    *p_stream<<xStr;
    xStr = QString("%1").arg(RH, 13, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(0, 7);
    *p_stream<<xStr;
    xStr = QString("%1").arg(Thicknes, 13, 'f', 5);
    *p_stream<<xStr;
    xStr = QString("%1").arg(0, 7);
    *p_stream<<xStr;
    *p_stream<<"\n";
    return true;
}

bool Little_r_fm12::writeEnding()
{
    *p_stream<<"-777777.00000      0-777777.00000      0      1.00000      0-888888.00000      0-888888.00000      0-888888.00000      0-888888.00000      0-888888.00000      0-888888.00000      0-888888.00000      0";
    *p_stream<<"\n";
    return true;
}

bool Little_r_fm12::writeTail(int a, //Number of valid fields in the report
                              int b, //Number of errors encountered during the decoding of the report
                              int c) //Number of warnings encountered during the decoding the report
{
    QString xStr;
    xStr = QString("%1").arg(a, 7);
    *p_stream<<xStr;
    xStr = QString("%1").arg(b, 7);
    *p_stream<<xStr;
    xStr = QString("%1").arg(c, 7);
    *p_stream<<xStr;
    *p_stream<<"\n";
    return true;
}
