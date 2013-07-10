#include "xml_namelist.h"

xml_namelist::xml_namelist(QObject *parent) :
    QObject(parent)
{
    p_domDocument = new QDomDocument;
}

bool xml_namelist::ReadXml(QString file_name)
{
    curXML=file_name;
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

void xml_namelist::SetDomDocument(QDomDocument *p_doc)
{
    p_domDocument = p_doc;
}

bool xml_namelist::writeasciiWps(QString path_name)
{
    QFile file(path_name+"namelist.wps"); //Файл для записи
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug()<<tr("Cannot write file %1:\n%2.")
                  .arg("namelist.wps")
                  .arg(file.errorString());
        return false;
    }
    QTextStream out(&file); //Текстовый поток для записи в файл
    QDomElement child = p_domDocument->documentElement().firstChildElement("folder"); //Первый уровень иерархии (WPS,WRF,WRFDA)
    QDomElement child_wps = child.firstChildElement("folder"); //Второй уровень иерархии первый элемент WPS
    while (!child_wps.isNull()) {
        writeFolderElement(child_wps, out);
        child_wps = child_wps.nextSiblingElement("folder");
    }
    return true;
}

bool xml_namelist::writeasciiWrf(QString path_name)
{
    QFile file(path_name+"namelist.input"); //Файл для записи
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug()<<tr("Cannot write file %1:\n%2.")
                  .arg("namelist.input")
                  .arg(file.errorString());
        return false;
    }
    QTextStream out(&file); //Текстовый поток для записи в файл
    QDomElement child = p_domDocument->documentElement().firstChildElement("folder"); //Первый уровень иерархии (WPS,WRF,WRFDA)
    child = child.nextSiblingElement("folder"); //Переход ко второму элементу
    QDomElement child_wrf = child.firstChildElement("folder"); //Второй уровень иерархии второй элемент WRF
    while (!child_wrf.isNull()) {
        writeFolderElement(child_wrf, out);
        child_wrf = child_wrf.nextSiblingElement("folder");
    }
    return true;
}

void xml_namelist::writeFolderElement(const QDomElement &element, QTextStream &device)
{
    device<<element.attribute("title")<<endl;
    QDomElement child = element.firstChildElement(); //Берем ребенка
    while (!child.isNull())
    {
        if (child.tagName() == "Key") //Если элемент значение, то разбираем
        {
            if(child.attribute("title")!="" && child.attribute("value")!="") //Если значения не ноль то пишем
                device <<" "<< child.attribute("title")<<" = "<<child.attribute("value")<<","<<endl;
        }
        child = child.nextSiblingElement(); //Переходим к следующему элементу
    }
    device<<"/"<<endl;
    device<<endl;
}

void xml_namelist::SetDates(QDate* p_start_date, QTime* p_start_time,QDate* p_end_date,QTime* p_end_time)
{
    QDomElement element = findElement(p_domDocument->documentElement(),"start_date"); //Ищем элеменет с title = start_date
    //WPS Начальная дата и время
    element.setAttribute("value", p_start_date->toString("yyyy-MM-dd_")+p_start_time->toString("hh:mm:ss")); //Меняем значение на заданное
    element = findElement(p_domDocument->documentElement(),"end_date");
    //WPS Конечная дата и время
    element.setAttribute("value", p_end_date->toString("yyyy-MM-dd_")+p_end_time->toString("hh:mm:ss"));
    element = findElement(p_domDocument->documentElement(),"start_year");
    //WRF Начальная дата
    element.setAttribute("value", p_start_date->toString("yyyy"));
    element = findElement(p_domDocument->documentElement(),"start_month");
    element.setAttribute("value", p_start_date->toString("MM"));
    element = findElement(p_domDocument->documentElement(),"start_day");
    element.setAttribute("value", p_start_date->toString("dd"));
    //WRF Начальное время
    element = findElement(p_domDocument->documentElement(),"start_hour");
    element.setAttribute("value", p_start_time->toString("hh"));
    element = findElement(p_domDocument->documentElement(),"start_minute");
    element.setAttribute("value", p_start_time->toString("mm"));
    element = findElement(p_domDocument->documentElement(),"start_second");
    element.setAttribute("value", p_start_time->toString("ss"));
    //WRF Конечная дата
    element = findElement(p_domDocument->documentElement(),"end_year");
    element.setAttribute("value", p_end_date->toString("yyyy"));
    element = findElement(p_domDocument->documentElement(),"end_month");
    element.setAttribute("value", p_end_date->toString("MM"));
    element = findElement(p_domDocument->documentElement(),"end_day");
    element.setAttribute("value", p_end_date->toString("dd"));
    //WRF Конечное время
    element = findElement(p_domDocument->documentElement(),"end_hour");
    element.setAttribute("value", p_end_time->toString("hh"));
    element = findElement(p_domDocument->documentElement(),"end_minute");
    element.setAttribute("value", p_end_time->toString("mm"));
    element = findElement(p_domDocument->documentElement(),"end_second");
    element.setAttribute("value", p_end_time->toString("ss"));
    //WRF run_days =
    int n_days = p_start_date->daysTo(*p_end_date);
    element = findElement(p_domDocument->documentElement(),"run_days");
    element.setAttribute("value", n_days);
    //WRF run_hours =
    element = findElement(p_domDocument->documentElement(),"run_hours");
    element.setAttribute("value", p_end_time->hour());
    //WRF run_minutes =
    element = findElement(p_domDocument->documentElement(),"run_minutes");
    element.setAttribute("value", p_end_time->minute());
    //WRF run_seconds =
    element = findElement(p_domDocument->documentElement(),"run_seconds");
    element.setAttribute("value", p_end_time->second());
    writeXML(curXML);
}

QDomElement xml_namelist::findElement(QDomElement parent, const QString &textToFind)
{
    for(QDomElement elem = parent.firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement())
    {
        if(elem.attribute("title")==textToFind) return elem;
      QDomElement e = findElement(elem, textToFind);
      if(!e.isNull()) return e;
    }
    return QDomElement();
}

bool xml_namelist::writeXML(QString file_name)
{
    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug()<<tr("Cannot write file %1:\n%2.")
                  .arg("namelist.input")
                  .arg(file.errorString());
        return false;
    }
    const int IndentSize = 4;
    QTextStream out(&file);
    p_domDocument->save(out, IndentSize);
    file.close();
    return true;
}

void xml_namelist::SetOutputPath(QString path)
{
    QDomElement opt_geogrid_tbl_path = findElement(p_domDocument->documentElement(),"opt_geogrid_tbl_path");
    opt_geogrid_tbl_path.setAttribute("value","'"+path+"'");
    QDomElement opt_metgrid_tbl_path = findElement(p_domDocument->documentElement(),"opt_metgrid_tbl_path");
    opt_metgrid_tbl_path.setAttribute("value","'"+path+"'");
    writeXML(curXML);
}
