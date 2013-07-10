#ifndef PSQLTOCSV_H
#define PSQLTOCSV_H

#include <QObject>
#include "QProcess"
#include "psqldriver.h"

class PsqlToCsv : public QObject
{
    Q_OBJECT
private:
    QString workDir;
    psqlDriver* db;
public:
    explicit PsqlToCsv(QObject *parent = 0);
    void writeAll();
    bool writeStation(int pgp_index);
    void setWorkDir(QString s_workdir);
    void setPsqlDriver(psqlDriver* psqldriver);
    
signals:
    void allDone(int,QProcess::ExitStatus); //сигнал об окончании работы
    
public slots:
    
};

#endif // PSQLTOCSV_H
