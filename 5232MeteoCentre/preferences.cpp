#include "preferences.h"

Preferences::Preferences(QObject *parent) :
    QObject(parent)
{
    work_path="";
    rzd_path="";
    rzd_filename="";
    site_path = "";
}

bool Preferences::readXml(const QString &fileName)
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
    work_path = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    rzd_path = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    rzd_filename = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    site_path = xmlReader.readElementText();
    file->close();
    return true;
}
