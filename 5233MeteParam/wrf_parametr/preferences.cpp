#include "preferences.h"

Preferences::Preferences(QObject *parent) :
    QObject(parent)
{
    netcdf1="";
    netcdf2 = "";
    output = "";
}

bool Preferences::writeXml(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Root");
    xmlWriter.writeTextElement("netcdf1", netcdf1);
    xmlWriter.writeTextElement("netcdf2", netcdf2);
    xmlWriter.writeTextElement("output", output);
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();
    return true;
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
    netcdf1 = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    netcdf2 = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    output = xmlReader.readElementText();
    file->close();
    return true;
}
