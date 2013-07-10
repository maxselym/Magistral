#include "psqldriver.h"
#include "QtXml"
#include "QtSql/QtSql"

psqlDriver::psqlDriver(QObject *parent) :
    QObject(parent)
{
    db = new QSqlDatabase;
}

bool psqlDriver::readPreferences(QString fileName)
{
    QFile *file = new QFile(fileName);
    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }
    QXmlStreamReader xmlReader(file);
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();
    server = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    dbname = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    user = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    pass = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    port = xmlReader.readElementText().toInt();
    file->close();
    return true;
}

bool psqlDriver::createConnection()
{
    QString driverName = "QPSQL";
    *db = QSqlDatabase::addDatabase(driverName, "main");
    db->setHostName(server);
    db->setDatabaseName(dbname);
    db->setUserName(user);
    db->setPassword(pass);
    db->setPort(port);
    if(!db->open())
    {
        return false;
    }
    return true;
}

bool psqlDriver::createTableRawPGP()
{
    if(!createConnection()){
        qDebug() << "Unable to create connection";
        return false;
    }
    QSqlQuery* query = new QSqlQuery(*db);
    QString str = "DROP TABLE raw_pgp";
    query->exec(str);
    str = "CREATE TABLE raw_pgp ("
            "seqno SERIAL PRIMARY KEY,"
            "pgp_index integer,"
            "mes_day date,"
            "mes_time time with time zone,"
            "t_air real,"
            "Press real,"
            "wind real,"
            "windmax real,"
            "winddir real,"
            "moist real,"
            "pcpt real);";
    if(!query->exec(str)){
        qDebug() << "Unable to create table";
        qDebug() << query->lastError();
        return false;
    }
    delete query;
    return true;
}


bool psqlDriver::insertIntoRawPGP(QStringList& list)
{
    QString str = "INSERT INTO raw_pgp(pgp_index, mes_day, mes_time, t_air, press, wind, windmax,"
            "winddir, moist, pcpt, tsoil5, tsoil20, tsoil40, tsoil80, trail)"
            "VALUES (:pgp_index, :mes_day, :mes_time, :t_air, :press, :wind, :windmax,"
            ":winddir, :moist, :pcpt,  :tsoil5, :tsoil20, :tsoil40, :tsoil80, :trail);";
    QSqlQuery* preparedQuery = new QSqlQuery(*db);


    QDate date =QDate::fromString(list[1],"d.M.yyyy");
    preparedQuery->prepare(str);
    preparedQuery->bindValue( ":pgp_index", list[0]);
    preparedQuery->bindValue( ":mes_day", date.toString("MM/dd/yyyy"));
    preparedQuery->bindValue( ":mes_time", list[2]);
    preparedQuery->bindValue( ":t_air", list[3]);
    preparedQuery->bindValue( ":press", list[4]);
    preparedQuery->bindValue( ":wind", list[5]);
    preparedQuery->bindValue( ":windmax", list[6]);
    preparedQuery->bindValue( ":winddir", list[7]);
    preparedQuery->bindValue( ":moist", list[8]);
    preparedQuery->bindValue( ":pcpt", list[9]);
    preparedQuery->bindValue( ":tsoil5", list[10]);
    preparedQuery->bindValue( ":tsoil20", list[11]);
    preparedQuery->bindValue( ":tsoil40", list[12]);
    preparedQuery->bindValue( ":tsoil80", list[13]);
    preparedQuery->bindValue( ":trail", list[14]);
    if(!preparedQuery->exec())
    {
        //qDebug() << "Unable to insert";
      //  qDebug() << preparedQuery.lastError().type();
        return false;
    }
    delete preparedQuery;
    return true;
}


/** Берем самую поледнюю строчку в mes_day
 * Если таблица пустая, то возвращаем текущую дату**/
QDate psqlDriver::GetLastDateTimeRawPGP()
{
    QDate date;
    QSqlQuery* query = new QSqlQuery(*db);
    if(query->exec("SELECT mes_day FROM raw_pgp order by mes_day desc limit 1"))
    {
        while(query->next()) {
            QString str = query->value(0).toString();
            return QDate::fromString(str,"yyyy-MM-dd");
        }
        return date.currentDate();

    } else
    {
        qDebug()<< query->lastError();
        return date.currentDate();
    }
    delete query;
}

QSqlDatabase *psqlDriver::GetDB()
{
    return db;
}


psqlDriver::~psqlDriver()
{
    if(db) {
        if(db->isOpen()) {
            db->close();
        }
        delete db;
    }
    QStringList list = QSqlDatabase::connectionNames();
    for(int i = 0; i < list.count(); ++i) {
        QSqlDatabase::removeDatabase(list[i]);
    }
}
