#ifndef PSQLDRIVER_H
#define PSQLDRIVER_H

#include <QObject>
#include "QDateTime"
#include "QtSql/QtSql"

class psqlDriver : public QObject
{
    Q_OBJECT
private:
    QString server;
    QString dbname;
    QString user;
    QString pass;
    int port;
    QSqlDatabase* db;


public:
    explicit psqlDriver(QObject *parent = 0);
    ~psqlDriver();
    bool readPreferences(QString fileName);
    bool createConnection();
    bool createTableRawPGP();
    bool insertIntoRawPGP(QStringList &list);
    QDate GetLastDateTimeRawPGP();
    QSqlDatabase* GetDB();
    
signals:
    
public slots:
    
};

#endif // PSQLDRIVER_H
