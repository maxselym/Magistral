#include "dbengine.h"
#include <QtSql>

DBEngine::DBEngine(QObject *parent) :
    QObject(parent)
{
    db = new QSqlDatabase;
    connectionName = "mydb";
}

DBEngine::~DBEngine()
{
}

bool DBEngine::createConnection()
{ 
    //Чтение параметров из preferences.xml
    connectionParametrs(host, port, dataBaseName, userName, password);

    //Подключаемся к базе
    QString driverName = "QPSQL";
    *db = QSqlDatabase::addDatabase(driverName,connectionName);
    db->setHostName(host);
    db->setDatabaseName(dataBaseName);
    db->setUserName(userName);
    db->setPassword(password);
    db->setPort(port);
    if(!db->open())
    {
        return false;
    }
    return true;
}

bool DBEngine::IsConnected()
{
    return db->isOpen();
}

void DBEngine::closeConnection()
{
    if(db->isOpen())
    {
        db->close();
        delete db;
        QSqlDatabase::removeDatabase(connectionName);
    }
}

QVector<int> DBEngine::GetStnIndexLst()
{
    QSqlQuery* query = new QSqlQuery(*db);
    query->exec("SELECT index FROM stn_list");
    QVector<int> vector;
    while (query->next()){
      vector<<query->value(0).toInt();
    }
    return vector;
}

QVector<QDate>* DBEngine::GetStnDateByIndex(int index)
{
    QVector<QDate>* vector = new QVector<QDate>;
    QSqlQuery* query = new QSqlQuery(*db);
    if(query->exec(QString("SELECT mes_day FROM raw_pgp WHERE pgp_index = %1 ORDER BY mes_day ASC").arg(index)))
    {
        while (query->next())
        {
            vector->append(query->value(0).toDate());
        }
        vector->erase(std::unique( vector->begin(), vector->end() ), vector->end()); //Удаляем дубликаты даты
        return vector;
    } else
    {
        qDebug()<<"Error on:"<<db->lastError();
        return vector;
    }
}

QVector<QTime>* DBEngine::GetStnTimeByIndex(int index, QDate &dt)
{
    QVector<QTime>* vector = new QVector<QTime>;
    QSqlQuery* query = new QSqlQuery(*db);
    if(query->exec(QString("SELECT mes_time FROM raw_pgp WHERE pgp_index = %1 AND mes_day = '%2' ORDER BY mes_time ASC").arg(index).arg(dt.toString("yyyy-MM-dd"))))
    {
        while (query->next())
        {
            vector->append(query->value(0).toTime());
        }
        return vector;
    } else
    {
        qDebug()<<"Error on:"<<db->lastError();
        return vector;
    }
}

bool DBEngine::GetStationCoordinates(int st_num, float &x, float &y, float &z)
{
    QSqlQuery* query = new QSqlQuery(*db);
    QString str;
    str.setNum(st_num);
    QString s_q = "SELECT * FROM stn_list WHERE index = " + str;

    if(query->exec(s_q))
    {
        query->next();
        x = query->value(2).toFloat();
        y = query->value(3).toFloat();
        z = query->value(4).toFloat();
        return true;
    } else
    {
        qDebug()<<"Error on:"<<s_q<<db->lastError();
        return false;
    }
}

QString DBEngine::GetStnNameByIndex(int index)
{
    QSqlQuery* query = new QSqlQuery(*db);
    QString str;
    str.setNum(index);
    QString s_q = "SELECT name FROM stn_list WHERE index = " + str;
    if(query->exec(s_q))
    {
        query->next();
        return query->value(0).toString();
    } else
    {
        qDebug()<<"Error on:"<<s_q<<db->lastError();
        return "Error!!!";
    }
}

bool DBEngine::GetStnInfoDyIndexAndTime(QDateTime dt, int index,
                                        float& t_air, float& press, float& wind, float& wind_dir, float &rh)
{
    QString st_num;
    st_num.setNum(index);
    QSqlQuery* query = new QSqlQuery(*db);
    if(db->tables().contains("raw_pgp"))
    {
        QString s_q = "SELECT * FROM raw_pgp WHERE pgp_index="+ st_num+
                " AND mes_day='" + dt.date().toString("yyyy-MM-dd")+"'"+
                " AND mes_time='" + dt.time().toString("hh:mm:ss")+"'";
        if(query->exec(s_q))
        {
            query->next();
            if(query->isValid())
            {
                t_air =  query->value(4).toFloat();
                press =  query->value(5).toFloat();
                wind =  query->value(6).toFloat();
                wind_dir =  query->value(8).toFloat();
                rh =  query->value(10).toFloat();
                return true;
            }
        }
        else
        {
            qDebug()<<"Error on:"<<s_q<<db->lastError();
            return false;
        }
    }
    return false;
}

bool DBEngine::AlterStnRowByIndex(QString name, QString x, QString y, QString z, QString s_index)
{
    QSqlQuery* query = new QSqlQuery(*db);
    QString sql = QString("UPDATE stn_list SET name='%1', x=%2, y=%3, z=%4 WHERE index=%5")
            .arg(name)
            .arg(x)
            .arg(y)
            .arg(z)
            .arg(s_index);
    if(!query->exec(sql))
    {
        qDebug()<<"Error on:"<<db->lastError();
        return true;
    }

    return true;
}

float DBEngine::GetRailT(QString st_num, QDateTime dt)
{
    QSqlQuery* query = new QSqlQuery(*db);
    if(db->tables().contains("raw_pgp"))
    {
        QString s_q = "SELECT * FROM raw_pgp WHERE pgp_index="+ st_num+
                " AND mes_day='" + dt.date().toString("yyyy-MM-dd")+"'"+
                " AND mes_time='" + dt.time().toString("hh:mm:ss")+"'";
        float t = -5000;
        if(query->exec(s_q))
        {
            query->next();
            if(query->isValid())
            {
                return query->value(4).toFloat();
            } else return t;
        }
        else
        {
            qDebug()<<"Error on:"<<s_q<<db->lastError();
            return t;
        }
    } else return -5000;
}

void DBEngine::connectionParametrs(QString &host, int &port, QString &dataBaseName, QString &userName, QString &password)
{
    //Открываем preferences.xml
    QFile *file = new QFile("preferences.xml");
    //Если нет, то выходим
    if (!file->open(QFile::ReadOnly | QFile::Text)) {return;}
    //Читаем xml
    QXmlStreamReader xmlReader(file);
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();
    host = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    dataBaseName = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    userName = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    password = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    port = xmlReader.readElementText().toInt();
    file->close();
}


QSqlError DBEngine::LastError()
{
    return db->lastError();
}


QSqlDatabase *DBEngine::GetDB()
{
    return db;
}
