#ifndef DBENGINE_H
#define DBENGINE_H

#include <QObject>
#include <QtSql>

/** Касс для работы с базой данных*/
class DBEngine : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase* db;
   // QSqlQuery* query;
    QSqlQueryModel *model;
    QString host;
    int port;
    QString dataBaseName;
    QString userName;
    QString password;
    QString connectionName;
public:
    /** Конструктор*/
    explicit DBEngine(QObject *parent = 0);

    ~DBEngine();
    /** Чтение параметров соединения из XML*/
    static void connectionParametrs(QString &host, int &port, QString &dataBaseName, QString &userName, QString &password);
    /** Соединение с БД*/
    bool createConnection();
    bool IsConnected();
    void closeConnection();
    QVector<int> GetStnIndexLst(); //Получить все индексы станций
    QVector<QDate> *GetStnDateByIndex(int index); //Получить имеющиеся дни по индексу
    QVector<QTime> *GetStnTimeByIndex(int index, QDate& dt); //Получить имеющиеся часы по индексу и дню
    bool GetStationCoordinates(int st_num, float &x, float &y, float &z);
    /** Получить имя станции по индексу**/
    QString GetStnNameByIndex(int index);
    bool GetStnInfoDyIndexAndTime(QDateTime dt, int index, float &t_air, float &press, float &wind, float &wind_dir, float &rh);
    bool AlterStnRowByIndex(QString name, QString x, QString y, QString z, QString s_index);
    float GetRailT(QString st_num, QDateTime dt);
    QSqlDatabase* GetDB();
    QSqlError LastError();

signals:

public slots:

};

#endif // DBENGINE_H
