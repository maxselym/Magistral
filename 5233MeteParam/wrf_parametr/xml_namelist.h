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
    QString curXML;
    bool writeXML(QString file_name);
    QDomElement findElement(QDomElement parent, const QString &textToFind);
    void writeFolderElement(const QDomElement &element, QTextStream &device);
public:
    explicit xml_namelist(QObject *parent = 0);
    void SetDates(QDate* p_start_date, QTime* p_start_time,QDate* p_end_date,QTime* p_end_time);
    void SetOutputPath(QString path);
    bool ReadXml(QString file_name);
    void SetDomDocument(QDomDocument*);
    bool writeasciiWps(QString);
    bool writeasciiWrf(QString);

signals:

public slots:

};

#endif // XML_NAMELIST_H
