#ifndef METEOMESSAGE_H
#define METEOMESSAGE_H

#include <QObject>
#include "preferences.h"
#include "QSqlDatabase"
#include "QProcess"
#include "QList"

/** Основной класс программы*/
class MeteoMessage : public QObject
{
    Q_OBJECT
public:
    /** Конструктор */
    explicit MeteoMessage(QObject *parent = 0);
    /** Класс соединения с базой данных*/
    QSqlDatabase db;
    /** Чтение параметров соединение из файла preferences.xml
     * и соединение с базой данных*/
    bool connectDB();
    QList<QString> GetStations(); /**<  Список станций*/
    /** Получить индентификатор предупреждения*/
    int GetMeteo(QString station);
    /** Получить предупреждение из базы по индентификатору*/
    QString GetReport(int n);
    /** Написать XML сообщение с предупреждением об ОЯ*/
    void writeXml(const QString &fileName, QString stn, QString text);
    preferences* pref;  /**<  Указатель на файл класс с настройками*/

signals:
    void allDone(int); /**<  Сигнал для завершения приложения*/
public slots:

};

#endif // METEOMESSAGE_H
