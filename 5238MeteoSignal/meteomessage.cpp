#include "meteomessage.h"
#include "QDebug"
#include "QtSql"
#include "qmath.h"


MeteoMessage::MeteoMessage(QObject *parent) :
    QObject(parent)
{
    pref = new preferences;
    QObject::connect(this,SIGNAL(allDone(int)),this->parent(),
             SLOT(quit()));
}

bool MeteoMessage::connectDB()
{
    if(!pref->readXml("preferences.xml"))
    {
        /** Не могу соединиться с БД*/
        qDebug()<<"Error read preferences file";
        return false;
    }

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(pref->hostName);
    db.setDatabaseName(pref->dataBaseName);
    db.setUserName(pref->userName);
    db.setPassword(pref->password);
    db.setPort(pref->port.toInt());

    if (!db.open()) {
        qDebug()<<tr("Database Error");
        qDebug()<<db.lastError().text();
        emit allDone(5);
        return false;
    }
    return true;
}


int MeteoMessage::GetMeteo(QString station)
{
    bool ret = false;

    if(!db.isOpen())
    {
        ret = connectDB();
    }

    QString outLine;
    QString findr;
    QSqlQuery que;
    findr = "select Wind_X Wind_Y T_A from meteo_table where ";
    findr += "Src";
    findr += " like '%";
    findr += station;
    findr += " %' ;";
    int Wind_x = 0;
    int Wind_y = 0;
    int T_A;
    if (ret)
    {

        que.exec(findr);

        if( que.next())
        {
            Wind_x = que.value(0).toInt();  //Берем поле 0 из селекта
            Wind_y = que.value(1).toInt(); //Берем поле 1 из селекта
            T_A = que.value(2).toInt(); //Берем поле 2 из селекта
        }
    }
    int speed = qSqrt(Wind_x*Wind_x+Wind_y*Wind_y);

    findr = "select Wind_max Temp_max from meteo_table where ";
    findr += "Src";
    findr += " like '%";
    findr += station;
    findr += " %' ;";
    int Wind_max;
    int Temp_max;
    if (ret)
    {

        que.exec(findr);

        if( que.next())
        {
            Wind_max = que.value(0).toInt();  //Берем поле 0 из селекта
            Temp_max = que.value(1).toInt(); //Берем поле 1 из селекта
        }
    }
    if(T_A>Temp_max && speed<Wind_max)
        return 1;
    else if(speed>Wind_max && T_A<Temp_max)
        return 2;
    else if(speed>Wind_max && T_A>Temp_max)
        return 3;
    return 0;

}

QList<QString> MeteoMessage::GetStations()
{
    bool ret = false;

    if(!db.isOpen())
    {
        ret = connectDB();
    }
    QString findr;
    QList<QString> stn_list;
    QSqlQuery que;
    findr = "select Seqno from Sources";
    if (ret)
    {

        que.exec(findr);

        while( que.next())
        {
            stn_list.append(que.value(0).toString());
        }
    }
    return stn_list;
}

void MeteoMessage::writeXml(const QString &fileName, QString stn, QString text)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Msg");
    xmlWriter.writeTextElement("id", stn);
    xmlWriter.writeTextElement("text", text);
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();
}

QString MeteoMessage::GetReport(int n)
{
    bool ret = false;

    if(!db.isOpen())
    {
        ret = connectDB();
    }
    QString findr;
    QList<QString> stn_list;
    QSqlQuery que;
    findr = "select report from reports where id = ";
    findr+= QString("%1").arg(n);
    if (ret)
    {

        que.exec(findr);

        if( que.next())
        {
            return que.value(0).toString();
        }
    }
    return "No danger reports";
}

