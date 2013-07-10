#include "wpsrun.h"


WPSRun::WPSRun(QObject *parent) :
    QObject(parent)
{
    n_proc = 0;
    QObject::connect(this,SIGNAL(allDone(int)),this->parent(),SLOT(quit()),
             Qt::QueuedConnection); //Qt::QueuedConnection - поместить сигнал в очередь потому что сигнал о завершении вышлется раньше пуска цикла сигналов

}

bool WPSRun::initialize(QString sPreferenceFile)
{
    pref = new Preferences();
    if(!pref->readXml(sPreferenceFile))
    {
        qDebug()<<"Error reading preference.xml";
        return false;
    }
    if(!connectToDB())
    {
        qDebug()<<"Error connecting to db";
        return false;
    }
//    QDir::setCurrent(pref->work_path+"//In");
    ClearDir(pref->work_path);
    ftpclient = new ftpClient();
    csvtopsql = new csvToPsql();
    psqltocsv = new PsqlToCsv();
    return true;
}

bool WPSRun::connectToDB()
{
    pdriver = new psqlDriver;
    if(!pdriver->readPreferences("db.xml"))
    {
        qDebug()<<"Error read preferences db.xml";
        pdriver->readPreferences("db.xml");
        return false;
    }
    if(!pdriver->createConnection())
    {
        qDebug()<<"Error open DB";
        return false;
    }
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

void WPSRun::RunProc(QString output_root, int n)
{
    QString ss;
    switch(n) {
    case 0:
        n_proc++;
        ftpclient->setWorkDir(output_root);
        ftpclient->setFTPServer(pref->rzd_path); /** Указываем имя сервера **/
        ftpclient->setFileName(pref->rzd_filename); /** Указываем файл для скачки **/
        connect(ftpclient,SIGNAL(allDone(int,QProcess::ExitStatus)),this,SLOT(Finished(int,QProcess::ExitStatus)));
        ftpclient->connectOrDisconnect();        
        break;
    case 1:
        n_proc++;
        //"/usr/local/bin/7za e *.zip"
        proc->start("7z e *.zip");
        break;
    case 2:
        n_proc++;
        csvtopsql->setWorkDir(output_root);
        csvtopsql->setPsqlDriver(pdriver);
        connect(csvtopsql,SIGNAL(allDone(int,QProcess::ExitStatus)),this,SLOT(Finished(int,QProcess::ExitStatus)));
        csvtopsql->importCsv("last72hrs.txt");
        break;
    case 3:
        n_proc++;
        psqltocsv->setWorkDir(pref->site_path);
        psqltocsv->setPsqlDriver(pdriver);
        connect(psqltocsv,SIGNAL(allDone(int,QProcess::ExitStatus)),this,SLOT(Finished(int,QProcess::ExitStatus)));
        psqltocsv->writeAll();
        break;
    default:
        delete pdriver;
        qDebug()<<"End time: "<<QDateTime::currentDateTime().toString("hh:mm:ss_dd.MM.yyyy");
        emit allDone(0); //sending the signal to stop the app
    };
}

void WPSRun::StartProc()
{
    proc = new QProcess();
    proc->setWorkingDirectory(pref->work_path); //рабочая директория
    connect(proc,SIGNAL(readyReadStandardOutput()),this, SLOT(ReadStdout()));
    connect(proc,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(Finished(int,QProcess::ExitStatus)));
    RunProc(pref->work_path, n_proc);
}

void WPSRun::Finished(int n, QProcess::ExitStatus)
{
    if(n == 0)
    {
        qDebug()<<QString("Module %1 finished!").arg(n_proc);
        RunProc(pref->work_path, n_proc);
    }
    else
    {
        qDebug()<<"Error!!!";
        emit allDone(5); //sending the signal to stop the app
    }
}

void WPSRun::ReadStdout()
{
    qDebug()<<proc->readAllStandardOutput();
}
