#include "preferences.h"

preferences::preferences(QObject *parent) :
    QObject(parent)
{
}

bool preferences::readXml(const QString &fileName)
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
    hostName = xmlReader.readElementText();
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
    port = xmlReader.readElementText();

    file->close();
    return true;
}
