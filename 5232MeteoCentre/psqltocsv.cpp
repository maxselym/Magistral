#include "QtSql/QtSql"
#include "psqltocsv.h"
#include "QFile"
#include "QDateTime"
#include "QTextStream"

PsqlToCsv::PsqlToCsv(QObject *parent) :
    QObject(parent)
{
}

void PsqlToCsv::writeAll()
{
    QSqlDatabase* database = db->GetDB();
    QSqlQuery* query = new QSqlQuery(*database);
    if(query->exec("SELECT * FROM stn_list"))
    {
        while(query->next())
        {
            if(writeStation(query->value(0).toInt()))
            {
                qDebug()<< "Station "<<query->value(0).toInt()<< "written!";
            }
        }
    } else
    {
        qDebug()<< query->lastError();
    }
    delete query;
    emit allDone(0,QProcess::NormalExit);
}

bool PsqlToCsv::writeStation(int pgp_index)
{
    QString f_T2TRail = workDir + QString("%1T2TRail.csv").arg(pgp_index);
    QString f_Moist = workDir + QString("%1Moist.csv").arg(pgp_index);
    QFile fileT2TRail(f_T2TRail);
    QFile fileMoist(f_Moist);

    QSqlDatabase* database = db->GetDB();
    QSqlQuery* query = new QSqlQuery(*database);
    QString str_query = QString("SELECT mes_day, mes_time, t_air, trail, moist FROM raw_pgp WHERE pgp_index = %1").arg(pgp_index);
    if(query->exec(str_query))
    {
        if(query->size() == 0) // Если select вернул пустоту - выходим
        {
            return false;
        }
        fileT2TRail.remove();
        fileMoist.remove();
        if(!fileT2TRail.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug()<<fileT2TRail.errorString();
            return false;
        }
        QTextStream outT2Trail(&fileT2TRail);
        outT2Trail.setCodec("UTF8");
        QString headT2Rail =  "Date,Т воздуха, Т рельсов\n";  //Для русского языка либо через tr, либо через qstring
        outT2Trail << headT2Rail;
        if(!fileMoist.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug()<<fileT2TRail.errorString();
            return false;
        }
        QTextStream outMoist(&fileMoist);
        outMoist << tr("Date, Осадки\n"); //Для русского языка либо через tr, либо через qstring
        while(query->next()) {
            outT2Trail << query->value(0).toDate().toString("yyyy/MM/dd")<< query->value(1).toTime().toString(" hh:mm")<< ",";
            outT2Trail << query->value(2).toFloat() << ",";
            outT2Trail << query->value(3).toFloat() << "\n";
            outMoist << query->value(0).toDate().toString("yyyy/MM/dd")<< query->value(1).toTime().toString(" hh:mm")<< ",";
            outMoist << query->value(4).toFloat() << "\n";

        }
    } else
    {
        qDebug()<< query->lastError();
        return false;
    }
    fileT2TRail.close();
    fileMoist.close();
    delete query;
    return true;
}

void PsqlToCsv::setWorkDir(QString s_workdir)
{
    workDir = s_workdir;
}

void PsqlToCsv::setPsqlDriver(psqlDriver *psqldriver)
{
    db = psqldriver;
}
