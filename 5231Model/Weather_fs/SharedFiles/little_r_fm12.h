#ifndef LITTLE_R_FM12_H
#define LITTLE_R_FM12_H

#include <QObject>
#include "QDateTime"
#include "QFile"
#include <QTextStream>
#include "SharedFiles/dbengine.h"

class Little_r_fm12 : public QObject
{
    Q_OBJECT
    /** Указатель на класс по работе с таблцей**/
public:
    static const float NoData = -888888.;
    explicit Little_r_fm12(QObject *parent = 0);
    QString s_name;
    float f_lon;
    float f_lat;
    QDateTime date_time;
    bool OpenFile(QString s_file);
    bool CloseFile();
    QTextStream* p_stream;
    QFile* p_file;
    bool writeRZD_DB_data(int index, QDateTime date);
    bool writeHeader(float f_lat, float f_lon, QString ID, QString Name, QString Platform, QString Source, float f_elevation, int num_vld__fld, int num_error, int num_warrning, int Seq_num, int num_dupd, bool is_sound, bool bogus, bool discard, int v_time_sut, int v_time_jul, QDateTime date, float slp, int slp_qc);
    bool writeData(float press, float height, float temp, float dew_point, float w_speed, float w_dir, float U, float V , float RH, float Thicknes);
    bool writeEnding();
    bool writeTail(int a, int b, int c);
signals:
    
public slots:
    
};

#endif // LITTLE_R_FM12_H
