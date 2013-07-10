#include "wpsrun.h"
#include "wrffileread.h"


WPSRun::WPSRun(QObject *parent) :
    QObject(parent)
{
    p_namelist_tool = new xml_namelist;
}

bool WPSRun::initialize(QString sPreferenceFile)
{
    pref = new Preferences();
    if(!pref->readXml(sPreferenceFile))
    {
        qDebug()<<"Error reading preference.xml";
        return false;
    }
    QDir::setCurrent(pref->output);
    if(!p_namelist_tool->ReadXml("namelist.wfs"))
        return false;
    return true;
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

void WPSRun::readpress1(QString file)
{
    WrfFileRead read;
    read.OpenFile(file);
    for(int i=0;i<read.num_metgrid_levels;i++)
        for(int j=0;i<read.south_north;i++)
            for(int k=0;i<read.west_east;i++)
                press1 << read.GetPressure(i,j,k);
}

void WPSRun::Analize()
{
    readpress1(pref->netcdf1);
    readpress1(pref->netcdf2);
    press1.append(press2);

    p_namelist_tool->writeasciiWps(pref->output);
    p_namelist_tool->writeasciiWrf(pref->output);
}

void WPSRun::readpress2(QString file)
{
    WrfFileRead read;
    read.OpenFile(file);
    for(int i=0;i<read.num_metgrid_levels;i++)
        for(int j=0;i<read.south_north;i++)
            for(int k=0;i<read.west_east;i++)
                press1 << read.GetPressure(i,j,k);
}
