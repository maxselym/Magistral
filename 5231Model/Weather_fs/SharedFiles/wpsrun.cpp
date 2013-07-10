#include "wpsrun.h"
#include "QProcess"
#include "little_r_fm12.h"


WPSRun::WPSRun(QObject *parent) :
    QObject(parent)
{
    p_namelist_tool = new xml_namelist(this);
    s_param = new special_param(this);
}

bool WPSRun::initialize()
{
    if(work_folder == "")
    {
        qDebug()<<"work_folder == """;
        return false;
    }

    ClearDir(work_folder);

    if(!p_namelist_tool->writeasciiWrf(work_folder))
    {
        qDebug()<<"Ошибка при создании namelist.wps";
        return false;
    }
    if(!p_namelist_tool->writeasciiWps(work_folder))
    {
        qDebug()<<"Ошибка при создании namelist.input";
        return false;
    }
    if(!p_namelist_tool->writeasciiOBSPROC(work_folder))
    {
        qDebug()<<"Ошибка при создании namelist.obsproc";
        return false;
    }
    if(!p_namelist_tool->writeasciiARWPost(work_folder))
    {
        qDebug()<<"Ошибка при создании namelist.ARWPost";
        return false;
    }
    ///////// WPS
    QFile::link(wps_root+"geogrid/src/geogrid.exe", work_folder+"geogrid.exe");
    QFile::link(wps_root+"geogrid/GEOGRID.TBL.ARW", work_folder+"GEOGRID.TBL");
    QFile::link(wps_root+"metgrid/src/metgrid.exe", work_folder+"metgrid.exe");
    QFile::link(wps_root+"metgrid/METGRID.TBL.ARW", work_folder+"METGRID.TBL");
    QFile::link(wps_root+"ungrib/src/ungrib.exe", work_folder+"ungrib.exe");
    QFile::link(wps_root+"ungrib/Variable_Tables/Vtable.GFS", work_folder+"Vtable");
    //////////END WPS
    ///////// WRF
    QDir dir(wrf_root+"run");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QFile::link(fileInfo.filePath(),work_folder+fileInfo.fileName());
    }
    QFile::link(wrf_root+"main/ndown.exe", work_folder+"ndown.exe");
    QFile::link(wrf_root+"main/nup.exe", work_folder+"nup.exe");
    QFile::link(wrf_root+"main/real.exe", work_folder+"real.exe");
    QFile::link(wrf_root+"main/tc.exe", work_folder+"tc.exe");
    QFile::link(wrf_root+"main/wrf.exe", work_folder+"wrf.exe");
    //////////END WRF
    QFile::link(wrf_root+"/MaxTools/mpd.hosts", work_folder+"mpd.hosts");

    QFile::link(wrf_root+"/MaxTools/myhosts", work_folder+"myhosts");

    // Link NCL files to draw images
    QFile::link(wrf_root+"/MaxTools/NCLrun.sh", work_folder+"NCLrun.sh");
    QFile::link(wrf_root+"/MaxTools/NCLscript.ncl", work_folder+"NCLscript.ncl");

    QFile::link(arwpost_root+"src/ARWpost.exe", work_folder+"ARWpost.exe");

    GribLink(grib_files,work_folder);

    //////////WRFDA
    QFile::link(wrfda_root+"/var/da/da_wrfvar.exe", work_folder+"da_wrfvar.exe"); //Ассимиляция
    QFile::link(wrfda_root+"/var/run/be.dat.cv3", work_folder+"be.dat"); //link background error statistics as be.dat


    QFile::link(wrfda_root+"/var/da/da_update_bc.exe", work_folder+"da_update_bc.exe"); //Обновление граничных условий
    QFile::link(wrf_root+"/MaxTools/parame.in", work_folder+"parame.in"); // namelist для da_update_bc.exe

    QFile::link(wrfda_root+"/var/obsproc/src/obsproc.exe", work_folder+"obsproc.exe"); //Препроцессинг метеоинформации в LITTLE_R
    QFile::link(wrfda_root+"/var/obsproc/obserr.txt", work_folder+"obserr.txt"); //Препроцессинг метеоинформации в LITTLE_R
    QString filename = work_folder + "ob.little_r";
    Little_r_fm12* little_r_fm12 = new Little_r_fm12(this); /**< указатель на Класс для записи в Little_r приземных наблюдений*/
    little_r_fm12->OpenFile(filename); //Открываем файл для записи

    QDateTime date = p_namelist_tool->GetDateTimeStart();

    for(int index=26001;index<26008;index++)
    {
        little_r_fm12->writeRZD_DB_data(index,date); //Пишем станцию
    }
    little_r_fm12->CloseFile(); // Закрываем файл

    //////////END WRFDA
    return true;
}

void WPSRun::SetTimeDateFromGRIB()
{
    p_namelist_tool->SetTimeDateFromGRIB(grib_files);
}

void WPSRun::SetStartEndDateTime(QString start, QString end)  //Формат "yyyy-MM-dd_hh:mm:ss"
{
    QDateTime curStart = QDateTime::fromString(start, "yyyy-MM-dd_hh:mm:ss");
    QDateTime curEnd = QDateTime::fromString(end, "yyyy-MM-dd_hh:mm:ss");
    p_namelist_tool->SetDates(&curStart,&curEnd);
}

void WPSRun::SetTimeDateStarttoCurrent()
{
    //Начинаем с 00:00 текущего дня
    //Считаем заданный промежуток
//    QDateTime curStart = QDateTime::fromString("2012-09-15_00:00:00", "yyyy-MM-dd_hh:mm:ss");
//    QDateTime curEnd = QDateTime::fromString("2012-09-15_06:00:00", "yyyy-MM-dd_hh:mm:ss");
    //Считаем 1 день
    QDateTime curStart = QDateTime::currentDateTime();
    QTime ntime = curStart.time();
    ntime.setHMS(GetPrevSrok(ntime.hour()),0,0);
    curStart.setTime(ntime);

    //curStart = curStart.addDays(1);  // Считаем со следующих суток
    QDateTime curEnd = curStart.addDays(1); //Одни сутки
    p_namelist_tool->SetDates(&curStart,&curEnd);
}

QString WPSRun::GetTimeDateRUN()
{
    QString str;
    str+=p_namelist_tool->GetDateTimeStart().toString("yyyy-MM-dd_hh:mm:ss");
    str+="\n";
    str+= p_namelist_tool->GetDateTimeEnd().toString("yyyy-MM-dd_hh:mm:ss");
    return str;
}
void WPSRun::ClearDir(const QString sDir)
{
    QDir dir(sDir);
    QFileInfoList list = dir.entryInfoList();
    for (int iList=0;iList<list.count();iList++)
    {
        QFileInfo info = list[iList];
        QString sFilePath = info.filePath();
        if (info.isDir())
        {
            // recursive
            if (info.fileName()!=".." && info.fileName()!=".")
            {
                ClearDir(sFilePath);
            }
        }
        else
        {
            // Do something with the file here
             QFile f(info.absoluteFilePath());
             f.remove();
        }
    }
}
void WPSRun::GribLink(QString sDir,QString output_root)
{
    int i1 =0;
    int i2 =0;
    int i3 =0;
    QDir dir(sDir);
    dir.setSorting(QDir::Type|QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    QDateTime* dt = new QDateTime();
    QDateTime StartDT = p_namelist_tool->GetDateTimeStart();
    QDateTime EndDT = p_namelist_tool->GetDateTimeEnd();
    QStringList dateList;

    int n;
    if(list.count()>100) //Если файлов очччень много, то берем последние 100
    {
        n = 100;
    } else
    {
        n = list.count();
    }
    for (int iList=0;iList<n;iList++)
    {

        QFileInfo info = list[iList];
        if(info.isFile())
        {
            QString sFilePath = info.filePath();
            p_namelist_tool->GetDateTimeFromGrib2(sFilePath,dt);
            if(*dt<StartDT || *dt>EndDT||dateList.contains(dt->toString("hhmmddMMyyyy")))
                continue;
            dateList<<dt->toString("hhmmddMMyyyy");

            QFile::link(sFilePath, output_root+"GRIBFILE."+Alphabet[i3]+Alphabet[i2]+Alphabet[i1]);
            i1++;
            if(i1>25)
            {
                i1 =0;
                i2++;
                if(i2>25)
                {
                    i2 =0;
                    i3++;
                    if(i3>25)
                    {
                        break;
                    }
                }
            }
        }
    }
}

bool WPSRun::ReadPreferenceXML(QString sFile)
{
    QFile file(sFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<tr("Cannot read file %1:\n%2.")
                             .arg(sFile)
                             .arg(file.errorString());
        return false;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine,&errorColumn))
    {
        qDebug()<<tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr);
        return false;
    }

    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "Root") {
        qDebug()<<tr("The file is not an WFS file.");
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "1.0") {
        qDebug()<<tr("The file is not an WFS version 1.0 "
                                    "file.");
        return false;
    }
    QDomElement child = root.firstChildElement(); //Берем первый элемент XML
    while (!child.isNull()) {
        if (child.tagName() == "Wps_root_folder")//Папка с WPS
        {wps_root = child.text();}
        else if (child.tagName() == "WRF_root_folder") //Папка с WRF
        {wrf_root = child.text();}
        else if (child.tagName() == "WRFDA_root_folder") //Папка с WRF
        {wrfda_root = child.text();}
        else if (child.tagName() == "ARWPost_root_folder") //Папка с ARWPost
        {arwpost_root = child.text();}
        else if (child.tagName() == "Wps_GRIB_folder") //Папка с файлами GRIB
        {grib_files = child.text();}
        else if (child.tagName() == "Wps_GEOG_folder") //Папка с файлами GEOG
        {geog_files = child.text();}
        else if (child.tagName() == "Wps_WORK_folder") //Рабочая папка
        {work_folder = child.text();}
        else if (child.tagName() == "WRF_output_folder") //Выходная папка
        {out_folder = child.text();}
        else if (child.tagName() == "WRF_Domain_to_export") //Выходная папка
        {domain_to_export = child.text();}
        else if (child.tagName() == "namelist_xml")
        {namelist_wps = child.text();}
        else if (child.tagName() == "Core_num")
        {Core_num = child.text().toInt();}
        child = child.nextSiblingElement(); //Переходим к следующему элементу
    }
    file.close();
    return true;
}

bool WPSRun::ReadWFS()
{
    if(!p_namelist_tool->ReadXml(namelist_wps))
        return false;
    /** Поправляем пути в конфигах*/
    p_namelist_tool->SetWorkFolder(work_folder);
    p_namelist_tool->SetOutputFolder(out_folder);
    p_namelist_tool->SetGeogPath(geog_files);
    return true;

}

int WPSRun::GetPrevSrok(int hours)
{
    for(int i = 0; i<=21; i=i+3)
    {
        if(hours == i)
            return hours;
    }
    hours = hours-1;
    return GetPrevSrok(hours);
}

QString WPSRun::GetWorkFolder()
{
    return work_folder;
}

QString WPSRun::GetOutFolder()
{
    return out_folder;
}

int WPSRun::GetCoreNum()
{
    return Core_num;
}

bool WPSRun::MoveWrfOut(QString s_dir, QString& report)
{
    QDir currentDir = QDir(work_folder);
    QStringList files;
    QStringList filters;
    filters << "wrfout*";
    files = currentDir.entryList(filters,QDir::Files | QDir::NoSymLinks);
    QDir outDir = QDir(s_dir);
    if(!outDir.exists())
    {
        if(!outDir.mkdir(s_dir))
        {
            report = "Can not create directory ";
            report+=s_dir;
            return false;
        }
    }
    if(files.count() == 0)
    {
        report = "No wrfout* files in directory";
        return false;
    }
    for(int i=0;i<files.count();i++)
    {
        if(!currentDir.rename(work_folder+files[i],s_dir + files[i]))
        {
            report = "Can not move file ";
            report+=files[i];
            return false;
        }
        report.append(files[i]);
        report+="\n";
    }
    report += "moved to ";
    report += s_dir;
    return true;

}

bool WPSRun::SendAlltoDB(QString& report)
{
    QStringList domains;
    domains=domain_to_export.split(",");

    QDateTime dt = p_namelist_tool->GetDateTimeStart();
    QString str_dt = dt.toString("_yyyy-MM-dd_hh:mm:ss");
    QString str_wrf = "wrfout_d0";
    for (int i =0; i< domains.count();i++)
    {
        // Do something with the tokens in domains
        QString str_dnum = domains.at(i);
        s_param = new special_param(this);
        QString f_name = str_wrf+str_dnum+str_dt;
        if(!s_param->OpenNetCDFFile(work_folder + f_name))
        {
            report = "Load NetCDF ERROR!!!";
            return false;
        }
        QVector<int> Vector_Stns = s_param->GetStnListInArea();
        for(int stn_num=0;stn_num<Vector_Stns.count();stn_num++)
        {
            for(int n_time=0;n_time<s_param->GetDimSize(0)-1;n_time++)
            {
                s_param->FillSpecarr(Vector_Stns[stn_num],n_time);
            }
        }
        report = "Data from "+ f_name + " moved to DB!";
    }
    return true;
}

QString WPSRun::GetWrfOutName(int n_domain)
{
    QString str_dnum;
    str_dnum.setNum(n_domain);

    QDateTime dt = p_namelist_tool->GetDateTimeStart();
    QString str_dt = dt.toString("_yyyy-MM-dd_hh:mm:ss");
    QString str_wrf = "wrfout_d0";
    return (str_wrf+str_dnum+str_dt);
}

QString WPSRun::GetOBSPROC3DVARName()
{
    //  obs_gts_2012-10-26_00:00:00.3DVAR
  QDateTime dt = p_namelist_tool->GetDateTimeStart();
  QString str_dt = dt.toString("_yyyy-MM-dd_hh:mm:ss");
  QString str_wrf = "obs_gts";
  QString str_3dvar = ".3DVAR";
  return (str_wrf+str_dt+str_3dvar);
}

bool WPSRun::LinkWrfout(QString link_name)
{
    QString f_name = GetWrfOutName(1);
    return QFile::link(work_folder+f_name, work_folder+link_name);
}

//QStringList WPSRun::GetWrfoutFiles()
//{
//    QDir currentDir = QDir(work_folder);
//    currentDir.setSorting(QDir::Name | QDir::Reversed);
//    QStringList files;
//    QStringList filters;
//    filters << "wrfout*";
//    files = currentDir.entryList(filters,QDir::Files | QDir::NoSymLinks);
//    return files;
//}
