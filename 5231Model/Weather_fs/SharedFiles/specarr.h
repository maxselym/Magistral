#ifndef SPECARR_H
#define SPECARR_H

#include <QObject>
#include "QtSql/QtSql"

class specArr : public QObject
{
    Q_OBJECT
public:
    explicit specArr(QObject *parent = 0);
    QString pgp_index;
    QDateTime date_time;
    float rainc;
    float rainnc;
    float hailnc;
    float snownc;
    float graupelnc;
    float snowh;
    float psfc;
    float t2;
    float windSpeed;
    float windAngle;
    float tRail;
    float tslb;
    float smois;
    int glaccyIce;
    int hurricane;
    bool CreateTable(QSqlDatabase* db);
    bool InsertValues(QSqlDatabase* db);

    
signals:
    
public slots:
    
};

#endif // SPECARR_H
