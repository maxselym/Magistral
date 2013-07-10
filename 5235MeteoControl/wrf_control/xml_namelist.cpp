#include "xml_namelist.h"

xml_namelist::xml_namelist(QObject *parent) :
    QObject(parent)
{
    p_domDocument = new QDomDocument;
}

bool xml_namelist::ReadXml(QString curXML)
{
    QString errorStr;
    int errorLine;
    int errorColumn;
    QFile file(curXML);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }
    if (!p_domDocument->setContent(&file, true, &errorStr, &errorLine,
                                &errorColumn)) {
        qDebug()<<tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }
    file.close();
    return true;
}

int xml_namelist::GetStationCount()
{
    QDomElement root = p_domDocument->documentElement();
    QDomElement child = root.firstChildElement("station");
    int n = 0;

    while (!child.isNull()) {
        n++;
        child = child.nextSiblingElement("station");
    }
    return n;
}

QString xml_namelist::GetStation(int st_num, int &x, int &y, int &z)
{
    QDomElement root = p_domDocument->documentElement();
    QDomElement child = root.firstChildElement("station");
    while (!child.isNull()) {
        if(child.attribute("stnum").toInt()==st_num)
        {
            x = child.attribute("x").toInt();
            y=child.attribute("y").toInt();
            z=child.attribute("z").toInt();
            return child.attribute("title");
        }
        child = child.nextSiblingElement("station");
    }
    return "";
}

