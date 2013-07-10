#include "ftpclient.h"
#include <QTextCodec>

ftpClient::ftpClient(QObject *parent) :
    QObject(parent), ftp(0)
{
    ftpServer = "";
    fileName = "";
}

void ftpClient::connectOrDisconnect()
{
    if (ftp) {
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
//![0]
        qDebug()<<"Connect";
        qDebug()<<"Please enter the name of an FTP server.";
        return;
    }

//![1]
    ftp = new QFtp(this);
    connect(ftp, SIGNAL(commandFinished(int,bool)),
            this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftp, SIGNAL(listInfo(QUrlInfo)),
            this, SLOT(addToList(QUrlInfo)));
//    connect(ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
//            this, SLOT(updateDataTransferProgress(qint64,qint64)));

//![1]

//![2]

    QUrl url(ftpServer);
    if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp")) {
        ftp->connectToHost(ftpServer, 21);
        ftp->login();
    } else {
        ftp->connectToHost(url.host(), url.port(21));

        if (!url.userName().isEmpty())
            ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
        else
            ftp->login();
        if (!url.path().isEmpty())
            ftp->cd(url.path());
    }
//![2]
    qDebug()<<(tr("Disconnect"));
    qDebug()<<(tr("Connecting to FTP server %1...").arg(ftpServer));
}

void ftpClient::ftpCommandFinished(int commandId, bool error)
{
    if (ftp->currentCommand() == QFtp::ConnectToHost) {
        if (error) {
            qDebug()<<"FTP";
            qDebug()<<tr("Unable to connect to the FTP server "
                      "at %1. Please check that the host "
                      "name is correct.").arg(ftpServer);
            connectOrDisconnect();
            return;
        }
        qDebug()<<tr("Logged onto %1.").arg(ftpServer);
        return;
    }
//![6]

//![7]
    if (ftp->currentCommand() == QFtp::Login)
        ftp->list();
//![7]

//![8]
    if (ftp->currentCommand() == QFtp::Get) {
        if (error) {
           qDebug()<<tr("Canceled download of %1.").arg(file->fileName());
            file->close();
            file->remove();
        } else {
            qDebug()<<tr("Downloaded %1 to current directory.").arg(file->fileName());
            file->close();
            if(!f_list.isEmpty())
                downloadFile(); //Если есть еще файлы, то качаем
            else
            {
                delete file;
                qDebug()<<"end!!!"; /// А тут высылаем сигнал о выходе

                emit allDone(0,QProcess::NormalExit);
            }

        }

//![8]
//![9]
    } else if (ftp->currentCommand() == QFtp::List) {
//        if (isDirectory.isEmpty()) {
//            qDebug()<<tr("File scan ended!");
            downloadFile(); //начинаем скачку файлов
 //       }
    }
    //![9]
}

void ftpClient::downloadFile()
{
    QString f_name = workDir + f_list.first();
    if (QFile::exists(f_name)) {
        qDebug()<<tr("A file with the name %1 already exists in "
                                    "the current directory.").arg(f_list.first());
        if(!QFile::remove(f_name))
        {
            qDebug()<<tr("Can not delete file with name %1 !").arg(f_list.first());
            cancelDownload();
            return;
        }
        else qDebug()<<tr("File with name %1 deleted").arg(f_list.first());
    }
//![4]
    file = new QFile(f_name);
    if (!file->open(QIODevice::WriteOnly)) {
        qDebug()<<tr("Unable to save the file %1: %2.").arg(f_list.first()).arg(file->errorString());
        delete file;
        return;
    }

    ftp->get(f_list.first(), file);
    qDebug()<<tr("Downloading %1...").arg(f_list.first());
    f_list.removeFirst();

}

void ftpClient::cancelDownload()
{
    emit allDone(1,QProcess::CrashExit);
}

//void ftpClient::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
//{
//    //qDebug()<<tr("totalBytes %1, readBytes %2").arg(totalBytes).arg(readBytes);
//}

void ftpClient::addToList(const QUrlInfo &urlInfo)
{
    if(urlInfo.isDir())
    {
        qDebug()<<tr("Folder on server %1").arg(urlInfo.name());
    }
    else
    {
        qDebug()<<tr("File on server %1").arg(urlInfo.name());
        f_list<<urlInfo.name(); /** Если имя не задано - заносим в список все без разбора **/
    }
}

void ftpClient::setFTPServer(QString s_server)
{
    ftpServer = s_server;
}

void ftpClient::setFileName(QString s_fname)
{
    fileName = s_fname;
}

void ftpClient::setWorkDir(QString s_workdir)
{
    workDir = s_workdir;
}
