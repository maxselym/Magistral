#include "csvtopsql.h"
#include "QFile"
#include "QTextStream"


csvToPsql::csvToPsql(QObject *parent) :
    QObject(parent)
{
    db = 0;
}

void csvToPsql::importCsv(QString fileName)
{
    if(db == 0)
    {
        qDebug()<<"DB driver not set";
        emit allDone(1,QProcess::CrashExit);
        return;
    }
    QFile inputFile(workDir + fileName);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"CSV File not found";
        emit allDone(1,QProcess::CrashExit);
        return;
    }
    QTextStream* in = new QTextStream(&inputFile);
    QString line;
    QDate dateTable = db->GetLastDateTimeRawPGP(); //Если таблица пустая, то возвращаем текущую дату
    qDebug()<<"Insert from "<<dateTable.toString("dd-MM-yyyy"); //Записываем данные начиная с этой даты
    line = in->readLine();
    int n_insert_fail = 0;
    int n_lines_count = 0;
    while (!line.isNull())
    {
        if(line[0]=='#')
        {
            line = in->readLine();
            continue;
        }
        QStringList lst = line.split(",");
        QDate dateRead =QDate::fromString(lst[1],"d.M.yyyy"); //Берем дату из файла
        if(dateRead>=dateTable) //Если дата больше начальной, то пишем
        {
            n_lines_count++;
            if(!db->insertIntoRawPGP(lst))
            {
                n_insert_fail++;
                //qDebug()<<"Error insert to db";
                //emit allDone(1,QProcess::CrashExit);
                //return;
            }
        }
        line = in->readLine();
    }
    qDebug()<<"Lines_count"<<n_lines_count;
    qDebug()<<"Skipped"<<n_insert_fail;
    emit allDone(0,QProcess::NormalExit);
}

void csvToPsql::setWorkDir(QString s_workdir)
{
    workDir = s_workdir;
}

void csvToPsql::setPsqlDriver(psqlDriver *psqldriver)
{
    db = psqldriver;
}
