#ifndef FTPCLIENT_H
#define FTPCLIENT_H
#include <QObject>
#include "QFile"
#include "QtNetwork/QFtp"
#include "QDebug"
#include "QStringList"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "QProcess"
class ftpClient : public QObject
{
    Q_OBJECT
public:
    explicit ftpClient(QObject *parent = 0);

signals:
private slots:
//    void updateDataTransferProgress(qint64 readBytes,
//                                    qint64 totalBytes);
    void addToList(const QUrlInfo &urlInfo);
public slots:
    void connectOrDisconnect();
    void downloadFile();
    void cancelDownload();

    void ftpCommandFinished(int commandId, bool error);
public:
    void setFTPServer(QString s_server);
    void setFileName(QString s_fname);
    void setWorkDir(QString s_workdir);
private:
    QStringList f_list;
    QFtp *ftp;
    QFile *file;
    QString ftpServer;
    QString fileName;
    QString workDir;
signals:
    void allDone(int,QProcess::ExitStatus); //we add a new signal

};

#endif // FTPCLIENT_H
