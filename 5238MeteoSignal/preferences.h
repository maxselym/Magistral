#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include "preferences.h"
#include "QXmlStreamReader"
#include "QFile"

/** Класс для чтения параметров соединения с БД*/
class preferences : public QObject
{
    Q_OBJECT
public:
    /** Конструктор*/
    explicit preferences(QObject *parent = 0);
    /** Прочесть XML файл*/
    bool readXml(const QString &fileName);
    QString hostName; /**< Строка соединения с базой*/
    QString dataBaseName; /**<  Имя базы данных*/
    QString userName; /**<   Имя пользователя*/
    QString password; /**<  Пароль пользователя*/
    QString port; /**<  Порт для соединения с БД*/

signals:

public slots:

};

#endif // PREFERENCES_H
