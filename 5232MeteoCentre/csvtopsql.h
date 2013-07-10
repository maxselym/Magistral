#ifndef CSVTOPSQL_H
#define CSVTOPSQL_H

#include <QObject>
#include "psqldriver.h"
#include "QProcess"
#include "QDebug"

class csvToPsql : public QObject
{
    Q_OBJECT
private:
    QString workDir;
    psqlDriver* db;
public:
    explicit csvToPsql(QObject *parent = 0);
    void importCsv(QString fileName);
    void setWorkDir(QString s_workdir);
    void setPsqlDriver(psqlDriver* psqldriver);
    
signals:
    void allDone(int,QProcess::ExitStatus); //сигнал об окончании работы
public slots:
    
};

#endif // CSVTOPSQL_H
