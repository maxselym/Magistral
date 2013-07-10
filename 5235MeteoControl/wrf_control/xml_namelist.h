#ifndef XML_NAMELIST_H
#define XML_NAMELIST_H

#include <QObject>
#include "QDomDocument"
#include "QFile"
#include "QDebug"
#include "QDate"

class xml_namelist : public QObject
{
    Q_OBJECT
private:
    QDomDocument* p_domDocument;
    QDomElement findElement(QDomElement parent, const QString &textToFind);
public:
    explicit xml_namelist(QObject *parent = 0);
    int GetStationCount();
    QString GetStation(int st_num, int& x,int& y,int& z);
    bool ReadXml(QString file_name);

signals:

public slots:

};

#endif // XML_NAMELIST_H
