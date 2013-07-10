#include "wpsrun_console.h"


wpsrun_console::wpsrun_console(QObject *parent) :
    QObject(parent)
{
    QObject::connect(this,SIGNAL(allDone(int)),this->parent(),SLOT(quit()),
             Qt::QueuedConnection);//Qt::QueuedConnection - поместить сигнал в очередь потому что сигнал о завершении вышлется раньше пуска цикла сигналов
}

void wpsrun_console::Start()
{
    proc->setWorkingDirectory(p_wpsrun->GetWorkFolder()); //рабочая директория
    proc->setProcessChannelMode(QProcess::MergedChannels); //Читаем весь вывод!
    connect(proc,SIGNAL(readyReadStandardOutput()),this, SLOT(ReadStdout())); //Если есть информация в стандарном выводе то вызваем readstdout
    connect(proc,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(Finished(int,QProcess::ExitStatus)));
    n_proc = 0;
    Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
}

void wpsrun_console::Run_proc(QString output_root, int n)
{
    QString report;
    switch(n) {
    case GEOGRID:   //0
        n_proc++;
        proc->start(output_root + "geogrid.exe");
        break;
    case UNGRIB:    //1
        n_proc++;
        proc->start(output_root + "ungrib.exe");
        break;
    case METGRID:   //2
        n_proc++;
        proc->start(output_root + "metgrid.exe");
        break;
    case REAL:      //3
        //proc->start(QString("mpirun -np %1 ").arg(p_wpsrun->GetCoreNum())+output_root + "real.exe");
        proc->start(QString("mpirun -np %1 --hostfile myhosts ").arg(p_wpsrun->GetCoreNum())+output_root + "real.exe");
        n_proc++;
        break;
    case OBSPROC:   //4
        n_proc++;
        proc->start(output_root + "obsproc.exe");
        break;
    case DA:        //5
        n_proc++;
        if(!QFile::link(output_root+"wrfinput_d01", output_root+"fg"))// link first guess file as fg
        {
            qDebug()<<"Error link wrfinput_d01 to fg";
            Finished(1,QProcess::CrashExit);
        }
        if(!QFile::link(output_root+p_wpsrun->GetOBSPROC3DVARName(), output_root+"ob.ascii"))// link OBSPROC processed observation file as ob.ascii
        {
            qDebug()<<"Error link "<<p_wpsrun->GetOBSPROC3DVARName()<<" to ob.ascii";
            Finished(1,QProcess::CrashExit);
        }
        proc->start(output_root + "da_wrfvar.exe");
        break;
    case UPDATEBC:  //6
        n_proc++;
        proc->start(output_root + "da_update_bc.exe");
        break;
    case WRF:       //7
        //proc->start(QString("mpirun -np %1 ").arg(p_wpsrun->GetCoreNum())+output_root + "wrf.exe");
        proc->start(QString("mpirun -np %1 --hostfile myhosts ").arg(p_wpsrun->GetCoreNum())+output_root + "wrf.exe");
        n_proc++;
        break;
    case 8:         //8
        if(!p_wpsrun->SendAlltoDB(report))
        {
            qDebug()<<(report);
            Finished(1,QProcess::CrashExit);
        } else
        {
            qDebug()<<(report);
            n_proc++;
            Finished(0,QProcess::NormalExit);
        }
        break;
    case 9: //Визуализация
        p_wpsrun->LinkWrfout("wrf.nc");
        proc->start(output_root + "NCLrun.sh");
        //proc->start(output_root + "ARWpost.exe");
        n_proc++;
        break;
    case 10:
        if(!p_wpsrun->MoveWrfOut(p_wpsrun->GetOutFolder(), report))
        {
             qDebug()<<(report);
             Finished(1,QProcess::CrashExit);
        } else
        {
            qDebug()<<(report);
            n_proc++;
            Finished(0,QProcess::NormalExit);
        }
        break;
    default:
        qDebug()<<("All DONE!!!");
        qDebug()<<"End time: "<<QDateTime::currentDateTime().toString("hh:mm:ss_dd.MM.yyyy");
        emit allDone(0);
    };
}

void wpsrun_console::ReadStdout()
{
    //qDebug()<<(proc->readAllStandardOutput()); //читаем стандартный вывод
}

void wpsrun_console::Finished(int n, QProcess::ExitStatus)
{
    if(n == 0)
    {
        qDebug()<<(QString("Module %1 finished!").arg(n_proc));
        Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
    }
    else
    {
        //qDebug()<<p_wpsrun->GetWorkFolder();
        qDebug()<<(QString("Error Module %1 !!!").arg(n_proc));
        emit allDone(5);
    }
}

void wpsrun_console::TimeFromGRIB()
{
  //  p_wpsrun->SetStartEndDateTime("2013-03-30_00:00:00","2013-03-30_06:00:00"); //Промежуток
   // p_wpsrun->SetTimeDateFromGRIB();          //По первому и последнему GRIB файлу
    p_wpsrun->SetTimeDateStarttoCurrent();    //От текущей даты на сутки вперед
    qDebug()<<("Date and time to calc:");
    qDebug()<<(p_wpsrun->GetTimeDateRUN());
}

bool wpsrun_console::Init()
{
    p_wpsrun = new WPSRun;
    proc = new QProcess(this);
    if(!p_wpsrun->ReadPreferenceXML("preferences.xml"))
    {
        qDebug()<<"Error reading preference.xml";
        emit allDone(5);
        return false;
    } else qDebug()<<("Preferences loaded!");
    if(!p_wpsrun->ReadWFS())
    {
        qDebug()<<"Error read WFS!!";
        emit allDone(5);
        return false;
    } else qDebug()<<("WFS loaded!");

    TimeFromGRIB();

    if(!p_wpsrun->initialize())
    {
        qDebug()<<"Error Link files!";
        emit allDone(5);
        return false;
    } else qDebug()<<("File linked!");
    qDebug()<<"Core num is: "<<p_wpsrun->GetCoreNum();
    return true;
}
