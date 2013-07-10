#include "xml_namelist.h"
#include "QProcess"
#include "QDir"
#include "qmath.h"
xml_namelist::xml_namelist(QObject *parent) :
    QObject(parent)
{
    p_domDocument = new QDomDocument;
}

bool xml_namelist::SetTimeDateFromGRIB(QString grib_files)
{
    QDateTime* start = new QDateTime;
    QDateTime* end = new QDateTime;
    bool first = true;
    QDir dir(grib_files);
    dir.setSorting(QDir::Type);
    QFileInfoList list = dir.entryInfoList();
    if(list.count()==0)
        return false;
    for (int iList=0;iList<list.count();iList++)
    {
        QFileInfo info = list[iList];
        if(info.isFile())
        {
            QString sFilePath = info.filePath();
            if(first)
            {
                GetDateTimeFromGrib2(sFilePath,start);
                first = false;
            }
            else
            {
                GetDateTimeFromGrib2(sFilePath,end);
            }
        }
    }
    SetDates(start,end);
    return true;
}

bool xml_namelist::GetDateTimeFromGrib2(QString fileName, QDateTime *dateTime)
{
    QProcess* proc = new QProcess;
    proc->start("wgrib2 -start_ft -d 1 "+ fileName,QIODevice::ReadWrite);

    // Wait for it to start
    if(!proc->waitForStarted())
        return false;

    // Continue reading the data until EOF reached
    QString data;

    while(proc->waitForReadyRead())
        data.append(proc->readAll());
    if(data == "")
    {
        return false;
    }
    else
    {
        /** Ищем в строке фразу start_ft= (9 символов) и читаем время
         * -start_ft        inv         verf time = reference_time + forecast_time (YYYYMMDDHH) - no stat. proc time
         * -start_FT        inv         verf time = reference_time + forecast_time (YYYYMMDDHHMMSS) - no stat. proc time*/

        int n = data.indexOf("start_ft=");
        *dateTime = QDateTime::fromString(data.mid(n+9,10), "yyyyMMddHH");//Берем из вывода только время
    }
    return true;
}

bool xml_namelist::ReadXml(QString file_name)
{
    curXML=file_name;
    QString errorStr;
    int errorLine;
    int errorColumn;
    QFile file(curXML);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<tr("Error open file WFS");
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
    /** Проверяем, есть ли директория, если нет, то пытаемся ее создать*/
    if(!QDir(path_name).exists())
    {
        if(!QDir().mkdir(path_name))
        {
            qDebug()<<tr("Cannot create directory: ")<<path_name;
            return false;
        }
    }
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
    /** Проверяем, есть ли директория, если нет, то пытаемся ее создать*/
    if(!QDir(path_name).exists())
    {
        if(!QDir().mkdir(path_name))
        {
            qDebug()<<tr("Cannot create directory: ")<<path_name;
            return false;
        }
    }
    QFile file(path_name+"namelist.input"); //Файл для записи
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug()<<tr("Cannot write file %1:\n%2.")
                  .arg("namelist.input")
                  .arg(file.errorString());
        return false;
    }
    QTextStream out(&file); //Текстовый поток для записи в файл
    QDomElement child = p_domDocument->documentElement().firstChildElement("folder"); //Первый уровень иерархии (WPS,WRF,OBSPROC,ARWPost)
    child = child.nextSiblingElement("folder"); //Переход ко 2-му элементу
    QDomElement child_wrf = child.firstChildElement("folder"); //Второй уровень иерархии первый элемент WRF
    while (!child_wrf.isNull()) {
        writeFolderElement(child_wrf, out);
        child_wrf = child_wrf.nextSiblingElement("folder");
    }
    return true;
}
bool xml_namelist::writeasciiOBSPROC(QString path_name)
{
    /** Проверяем, есть ли директория, если нет, то пытаемся ее создать*/
    if(!QDir(path_name).exists())
    {
        if(!QDir().mkdir(path_name))
        {
            qDebug()<<tr("Cannot create directory: ")<<path_name;
            return false;
        }
    }
    QFile file(path_name+"namelist.obsproc"); //Файл для записи
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug()<<tr("Cannot write file %1:\n%2.")
                  .arg("namelist.obsproc")
                  .arg(file.errorString());
        return false;
    }
    QTextStream out(&file); //Текстовый поток для записи в файл
    QDomElement child = p_domDocument->documentElement().firstChildElement("folder"); //Первый уровень иерархии (WPS,WRF,OBSPROC,ARWPOST)
    child = child.nextSiblingElement("folder"); //Переход ко 2-му элементу
    child = child.nextSiblingElement("folder"); //Переход ко 3-му элементу
    QDomElement child_wrf = child.firstChildElement("folder"); //Второй уровень иерархии первый элемент OBSPROC
    while (!child_wrf.isNull()) {
        writeFolderElement(child_wrf, out);
        child_wrf = child_wrf.nextSiblingElement("folder");
    }
    return true;
}
bool xml_namelist::writeasciiARWPost(QString path_name)
{
    /** Проверяем, есть ли директория, если нет, то пытаемся ее создать*/
    if(!QDir(path_name).exists())
    {
        if(!QDir().mkdir(path_name))
        {
            qDebug()<<tr("Cannot create directory: ")<<path_name;
            return false;
        }
    }
    QFile file(path_name+"namelist.ARWpost"); //Файл для записи
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug()<<tr("Cannot write file %1:\n%2.")
                  .arg("namelist.pinterp")
                  .arg(file.errorString());
        return false;
    }
    QTextStream out(&file); //Текстовый поток для записи в файл
    QDomElement child = p_domDocument->documentElement().firstChildElement("folder"); //Первый уровень иерархии (WPS,WRF,OBSPROC,ARWPost)
    child = child.nextSiblingElement("folder"); //Переход ко 2-му элементу
    child = child.nextSiblingElement("folder"); //Переход ко 3-му элементу
    child = child.nextSiblingElement("folder"); //Переход ко 4-му элементу
    QDomElement child_p_inter = child.firstChildElement("folder"); //Второй уровень иерархии первый элемент ARWPost
    while (!child_p_inter.isNull()) {
        writeFolderElement(child_p_inter, out);
        child_p_inter = child_p_inter.nextSiblingElement("folder");
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


void xml_namelist::SetDates(QDateTime* p_start, QDateTime* p_end)
{
    SetStartDateTime(p_start);
    SetEndDateTime(p_end);
    SetTimeAnalysis(p_start, 3600);
    /** Вычисляем количество часов между интервалами*/
    int sec_to_count = p_start->secsTo(*p_end);
    int hours = qFloor(sec_to_count/3600);

    QDomElement element;
    /** WRF run_days ставим 0*/
    element = findElement(p_domDocument->documentElement(),"run_days");
    element.setAttribute("value", 0);
    element = findElement(p_domDocument->documentElement(),"run_hours");
    element.setAttribute("value", hours);
    //WRF run_minutes =
    element = findElement(p_domDocument->documentElement(),"run_minutes");
    element.setAttribute("value", 0);
    //WRF run_seconds =
    element = findElement(p_domDocument->documentElement(),"run_seconds");
    element.setAttribute("value", 0);
    writeXML(curXML);
}

void xml_namelist::SetTimeAnalysis(QDateTime *p_start, int deltaInSeconds)
{
    QDomElement elementOBSPROC;
    QDomElement elementWRF;
    QDomElement element;
    QDateTime timeMin = p_start->addSecs(deltaInSeconds*(-1));
    QDateTime timeMax = p_start->addSecs(deltaInSeconds);

    QString time_window_min = timeMin.date().toString("yyyy-MM-dd_")+timeMin.time().toString("hh:mm:ss");
    QString time_window_max = timeMax.date().toString("yyyy-MM-dd_")+timeMax.time().toString("hh:mm:ss");
    QString analiseTime = p_start->date().toString("yyyy-MM-dd_")+p_start->time().toString("hh:mm:ss");
    /** WRF Начальная дата и время*/
    elementWRF = findElement(p_domDocument->documentElement(),"WRF"); //Ищем элеменет WPS
    element = findElement(elementWRF,"analysis_date"); //Ищем элеменет с title = start_date
    element.setAttribute("value", analiseTime); //Меняем значение на заданное
    element = findElement(elementWRF,"time_window_min"); //Ищем элеменет с title = start_date
    element.setAttribute("value", time_window_min); //Меняем значение на заданное
    element = findElement(elementWRF,"time_window_max"); //Ищем элеменет с title = start_date
    element.setAttribute("value", time_window_max); //Меняем значение на заданное
    /** WRF Начальная дата и время*/
    elementOBSPROC = findElement(p_domDocument->documentElement(),"OBSPROC"); //Ищем элеменет WPS
    element = findElement(elementOBSPROC,"time_analysis"); //Ищем элеменет с title = start_date
    element.setAttribute("value", analiseTime); //Меняем значение на заданное
    element = findElement(elementOBSPROC,"time_window_min"); //Ищем элеменет с title = start_date
    element.setAttribute("value", time_window_min); //Меняем значение на заданное
    element = findElement(elementOBSPROC,"time_window_max"); //Ищем элеменет с title = start_date
    element.setAttribute("value", time_window_max); //Меняем значение на заданное
}

void xml_namelist::GetParentIDAndRatio(QString &s_id, QString &s_ratio)
{
    QDomElement element_i_parent_start = findElement(p_domDocument->documentElement(),"parent_id"); //Ищем элеменет с title = ref_lat
    s_id = element_i_parent_start.attribute("value");
    QDomElement element_j_parent_start= findElement(p_domDocument->documentElement(),"parent_grid_ratio"); //Ищем элеменет с title = ref_lon
    s_ratio = element_j_parent_start.attribute("value");
}

void xml_namelist::SetParentIDAndRatio(QString s_id, QString s_ratio)
{
    QDomElement elementWPS = findElement(p_domDocument->documentElement(),"WPS"); //Ищем элеменет WPS
    QDomElement elementWRF = findElement(p_domDocument->documentElement(),"WRF"); //Ищем элеменет с title = WRF
    QDomElement element;
    element = findElement(elementWPS,"parent_id"); //Ищем элеменет с title = ref_lat
    element.setAttribute("value", s_id); //Меняем значение на заданное
    element = findElement(elementWPS,"parent_grid_ratio"); //Ищем элеменет с title = ref_lat
    element.setAttribute("value", s_ratio); //Меняем значение на заданное
    element = findElement(elementWRF,"parent_id"); //Ищем элеменет с title = ref_lat
    element.setAttribute("value", s_id); //Меняем значение на заданное
    element = findElement(elementWRF,"parent_grid_ratio"); //Ищем элеменет с title = ref_lat
    element.setAttribute("value", s_ratio); //Меняем значение на заданное
    writeXML(curXML);
}

void xml_namelist::SetDomainCount(int n_count)
{
    QDomElement elementWPS;
    QDomElement elementWRF;
    QDomElement element;
    /** WPS*/
    elementWPS = findElement(p_domDocument->documentElement(),"WPS"); //Ищем элеменет WPS
    element = findElement(elementWPS,"max_dom"); //Ищем элеменет с title = max_dom
    element.setAttribute("value", n_count); //Меняем значение на заданное
    /** WRF Начальная дата*/
    elementWRF = findElement(p_domDocument->documentElement(),"WRF"); //Ищем элеменет с title = WRF
    element = findElement(elementWRF,"max_dom");
    element.setAttribute("value", n_count);
}

int xml_namelist::GetDomainCount()
{
    QDomElement element_max_dom = findElement(p_domDocument->documentElement(),"max_dom"); //Ищем элеменет с title = max_dom
    return element_max_dom.attribute("value").toInt();
}

void xml_namelist::SetParentStart(QString s_i, QString s_j)
{
    QDomElement elementWPS = findElement(p_domDocument->documentElement(),"WPS"); //Ищем элеменет WPS
    QDomElement elementWRF = findElement(p_domDocument->documentElement(),"WRF"); //Ищем элеменет с title = WRF
    QDomElement element;

    element = findElement(elementWPS,"i_parent_start"); //Ищем элеменет с title = ref_lat
    element.setAttribute("value", s_i); //Меняем значение на заданное
    element = findElement(elementWPS,"j_parent_start"); //Ищем элеменет с title = ref_lat
    element.setAttribute("value", s_j); //Меняем значение на заданное
    element = findElement(elementWRF,"i_parent_start"); //Ищем элеменет с title = ref_lat
    element.setAttribute("value", s_i); //Меняем значение на заданное
    element = findElement(elementWRF,"j_parent_start"); //Ищем элеменет с title = ref_lat
    element.setAttribute("value", s_j); //Меняем значение на заданное
    writeXML(curXML);
}

void xml_namelist::GetParentStart(QString &s_i, QString &s_j)
{
    QDomElement element_i_parent_start = findElement(p_domDocument->documentElement(),"i_parent_start"); //Ищем элеменет с title = ref_lat
    s_i = element_i_parent_start.attribute("value");
    QDomElement element_j_parent_start= findElement(p_domDocument->documentElement(),"j_parent_start"); //Ищем элеменет с title = ref_lon
    s_j = element_j_parent_start.attribute("value");
}

void xml_namelist::SetStartDateTime(QDateTime* p_start)
{
    QDomElement elementWPS;
    QDomElement elementARWpost;
    QDomElement elementWRF;
    QDomElement element;
    /** Для одного домена */
    QString startTimeWps = p_start->date().toString("yyyy-MM-dd_")+p_start->time().toString("hh:mm:ss");
    QString startyyyy = p_start->date().toString("yyyy");
    QString startMM = p_start->date().toString("MM");
    QString startdd = p_start->date().toString("dd");
    QString starthh = p_start->time().toString("hh");
    QString startmm = p_start->time().toString("mm");
    QString startss = p_start->time().toString("ss");
    /** Для n доменов */
    for(int i = 1; i< GetDomainCount();i++)
    {
        startTimeWps += "," + p_start->date().toString("yyyy-MM-dd_")+p_start->time().toString("hh:mm:ss"); //Три раза повторяем время для трех доменов
        startyyyy += "," + p_start->date().toString("yyyy");
        startMM += "," + p_start->date().toString("MM");
        startdd += "," + p_start->date().toString("dd");
        starthh += "," + p_start->time().toString("hh");
        startmm += "," + p_start->time().toString("mm");
        startss += "," + p_start->time().toString("ss");

    }
    /** WPS Начальная дата и время*/
    elementWPS = findElement(p_domDocument->documentElement(),"WPS"); //Ищем элеменет WPS
    element = findElement(elementWPS,"start_date"); //Ищем элеменет с title = start_date
    element.setAttribute("value", startTimeWps); //Меняем значение на заданное
    /** ARWPost Начальная дата и время*/
    elementARWpost = findElement(p_domDocument->documentElement(),"ARWPost"); //Ищем элеменет с title = ARWPost
    element = findElement(elementARWpost,"start_date"); //Ищем элеменет с title = start_date
    element.setAttribute("value", startTimeWps); //Меняем значение на заданное
    /** WRF Начальная дата*/
    elementWRF = findElement(p_domDocument->documentElement(),"WRF"); //Ищем элеменет с title = WRF
    element = findElement(elementWRF,"start_year");
    element.setAttribute("value", startyyyy);
    element = findElement(elementWRF,"start_month");
    element.setAttribute("value", startMM);
    element = findElement(elementWRF,"start_day");
    element.setAttribute("value", startdd);
    /** WRF Начальное время*/
    element = findElement(elementWRF,"start_hour");
    element.setAttribute("value", starthh);
    element = findElement(elementWRF,"start_minute");
    element.setAttribute("value", startmm);
    element = findElement(elementWRF,"start_second");
    element.setAttribute("value", startss);
    writeXML(curXML);
}

void xml_namelist::SetEndDateTime(QDateTime *p_end)
{
    QDomElement elementWPS;
    QDomElement elementARWpost;
    QDomElement elementWRF;
    QDomElement element;
    /** Для одного домена */
    QString endTimeWps = p_end->date().toString("yyyy-MM-dd_")+p_end->time().toString("hh:mm:ss");
    QString endyyyy = p_end->date().toString("yyyy");
    QString endMM = p_end->date().toString("MM");
    QString enddd = p_end->date().toString("dd");
    QString endhh = p_end->time().toString("hh");
    QString endmm = p_end->time().toString("mm");
    QString endss = p_end->time().toString("ss");
    /** Для n доменов */
    for(int i = 1; i< GetDomainCount();i++)
    {
        endTimeWps += "," + p_end->date().toString("yyyy-MM-dd_")+p_end->time().toString("hh:mm:ss"); //Три раза повторяем время для трех доменов
        endyyyy += "," + p_end->date().toString("yyyy");
        endMM += "," + p_end->date().toString("MM");
        enddd += "," + p_end->date().toString("dd");
        endhh += "," + p_end->time().toString("hh");
        endmm += "," + p_end->time().toString("mm");
        endss += "," + p_end->time().toString("ss");
    }
    /** WPS Конечная дата и время*/
    elementWPS = findElement(p_domDocument->documentElement(),"WPS"); //Ищем элеменет WPS
    element = findElement(elementWPS,"end_date"); //Ищем элеменет с title = start_date
    element.setAttribute("value",endTimeWps); //Меняем значение на заданное
    /** ARWPost Конечная дата и время*/
    elementARWpost = findElement(p_domDocument->documentElement(),"ARWPost"); //Ищем элеменет с title = ARWPost
    element = findElement(elementARWpost,"end_date"); //Ищем элеменет с title = start_date
    element.setAttribute("value", p_end->date().toString("yyyy-MM-dd_")+p_end->time().toString("hh:mm:ss")); //Меняем значение на заданное
    /** WRF Конечная дата*/
    elementWRF = findElement(p_domDocument->documentElement(),"WRF"); //Ищем элеменет с title = WRF
    element = findElement(elementWRF,"end_year");
    element.setAttribute("value", endyyyy);
    element = findElement(elementWRF,"end_month");
    element.setAttribute("value", endMM);
    element = findElement(elementWRF,"end_day");
    element.setAttribute("value", enddd);
    /** WRF Конечное время*/
    element = findElement(elementWRF,"end_hour");
    element.setAttribute("value", endhh);
    element = findElement(elementWRF,"end_minute");
    element.setAttribute("value", endmm);
    element = findElement(elementWRF,"end_second");
    element.setAttribute("value", endss);
    writeXML(curXML);
}

void xml_namelist::SetCentreDomain(float ref_lat, float ref_lon)
{
    QString str_ref_lat = QString::number(ref_lat);
    QString str_ref_lon = QString::number(ref_lon);
    QDomElement element = findElement(p_domDocument->documentElement(),"ref_lat"); //Ищем элеменет с title = ref_lat
    //WPS Центр lat
    element.setAttribute("value", str_ref_lat); //Меняем значение на заданное
    element = findElement(p_domDocument->documentElement(),"ref_lon");
    //WPS Центр lon
    element.setAttribute("value", str_ref_lon);
    element = findElement(p_domDocument->documentElement(),"truelat1");
    element.setAttribute("value", ref_lat);
    element = findElement(p_domDocument->documentElement(),"truelat2");
    element.setAttribute("value", ref_lat);
    writeXML(curXML);
}

void xml_namelist::GetCentreDomain(float &ref_lat, float &ref_lon)
{
    QDomElement element_ref_lat = findElement(p_domDocument->documentElement(),"ref_lat"); //Ищем элеменет с title = ref_lat
    ref_lat = element_ref_lat.attribute("value").toFloat();
    QDomElement element_ref_lon = findElement(p_domDocument->documentElement(),"ref_lon"); //Ищем элеменет с title = ref_lon
    ref_lon = element_ref_lon.attribute("value").toFloat();
}

void xml_namelist::SetNumAndStep(QString x_num, QString dx, QString y_num, QString dy)
{
    QDomElement geogrid = findElement(p_domDocument->documentElement(),"&geogrid"); //Ищем элеменет geogrid
    QDomElement domains = findElement(p_domDocument->documentElement(),"&domains"); //Ищем элеменет geogrid
    QStringList dxlist = dx.split(",");
    QStringList dylist = dy.split(",");

    SetNumAndStepElement(geogrid,x_num,dxlist.at(0),y_num,dylist.at(0)); //В секцию geogrid только первые значения dx и dy
    SetNumAndStepElement(domains,x_num,dx,y_num,dy);
    QDomElement ref_x = findElement(p_domDocument->documentElement(),"ref_x"); //Ищем элеменет ref_x
    QDomElement ref_y = findElement(p_domDocument->documentElement(),"ref_y"); //Ищем элеменет ref_yum

    QStringList listx = x_num.split(",");
    QStringList listy = y_num.split(",");
    int n_ref_x = listx.at(0).toInt() /2;
    int n_ref_y = listy.at(0).toInt() /2;
    ref_x.setAttribute("value",QString::number(n_ref_x));
    ref_y.setAttribute("value",QString::number(n_ref_y));
    writeXML(curXML);
}

void xml_namelist::GetNumAndStep(QString &x_num, QString &dx, QString &y_num, QString &dy)
{
  //  QDomElement geogrid = findElement(p_domDocument->documentElement(),"&geogrid"); //Ищем элеменет geogrid
    QDomElement domains = findElement(p_domDocument->documentElement(),"&domains"); //Ищем элеменет geogrid


    QDomElement element_dx = findElement(domains,"dx"); //Ищем элеменет с title = dx
    dx = element_dx.attribute("value");
    QDomElement element_dy = findElement(domains,"dy"); //Ищем элеменет с title = dy
    dy = element_dy.attribute("value");
    QDomElement element_num_x = findElement(domains,"e_we"); //Ищем элеменет с title = e_we
    x_num = element_num_x.attribute("value");
    QDomElement element_num_y = findElement(domains,"e_sn"); //Ищем элеменет с title = e_sn
    y_num = element_num_y.attribute("value");
}
void xml_namelist::SetNumAndStepElement(const QDomElement &element, QString x_num, QString dx, QString y_num, QString dy)
{
    QDomElement element_dx = findElement(element,"dx");
    QDomElement element_dy = findElement(element,"dy");
    QDomElement element_num_x = findElement(element,"e_we");
    QDomElement element_num_y = findElement(element,"e_sn");
    element_dx.setAttribute("value",dx);
    element_dy.setAttribute("value",dy);
    element_num_x.setAttribute("value",x_num);
    element_num_y.setAttribute("value",y_num);
}

QDateTime xml_namelist::GetDateTimeStart()
{
    QDomElement element = findElement(p_domDocument->documentElement(),"start_date"); //Ищем элеменет с title = start_date
    QString str = element.attribute("value").mid(0,19);

    return QDateTime::fromString(str,"yyyy-MM-dd_hh:mm:ss");
}

QDateTime xml_namelist::GetDateTimeEnd()
{
    QDomElement element = findElement(p_domDocument->documentElement(),"end_date"); //Ищем элеменет с title = end_date
    QString str = element.attribute("value").mid(0,19);
    return QDateTime::fromString(str,"yyyy-MM-dd_hh:mm:ss");
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

void xml_namelist::SetWorkFolder(QString path)
{
    QDomElement opt_geogrid_tbl_path = findElement(p_domDocument->documentElement(),"opt_geogrid_tbl_path");
    opt_geogrid_tbl_path.setAttribute("value","'"+path+"'");
    QDomElement opt_metgrid_tbl_path = findElement(p_domDocument->documentElement(),"opt_metgrid_tbl_path");
    opt_metgrid_tbl_path.setAttribute("value","'"+path+"'");
    writeXML(curXML);
}

void xml_namelist::SetOutputFolder(QString path)
{
    QDomElement opt_geogrid_tbl_path = findElement(p_domDocument->documentElement(),"output_root_name");
    opt_geogrid_tbl_path.setAttribute("value","'"+path+"wrfpost_"+GetDateTimeStart().toString("yyyy-MM-dd_hh:mm")+"'");
    writeXML(curXML);
}

void xml_namelist::SetGeogPath(QString path)
{
    QDomElement opt_geogrid_tbl_path = findElement(p_domDocument->documentElement(),"geog_data_path");
    opt_geogrid_tbl_path.setAttribute("value","'"+path+"'");
    writeXML(curXML);
}
