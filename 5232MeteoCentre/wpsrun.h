#ifndef WPSRUN_H
#define WPSRUN_H

#include <QObject>
#include "psqldriver.h"
#include "QDebug"
#include "preferences.h"
#include "QDir"
#include "QProcess"
#include "ftpclient.h"
#include "csvtopsql.h"
#include "psqltocsv.h"


class WPSRun : public QObject
{
    Q_OBJECT

public:
    explicit WPSRun(QObject *parent = 0);
    bool initialize(QString sPreferenceFile);
    bool connectToDB();
    void ClearDir(const QString d);
    void StartProc();
    void RunProc(QString output_root, int n);
private:
    psqlDriver* pdriver;
    Preferences* pref;
    ftpClient* ftpclient; //Указатель на класс ftp-клиент
    csvToPsql* csvtopsql;
    PsqlToCsv* psqltocsv;
    int n_proc;
    QProcess* proc;
private slots:
    void ReadStdout();
    void Finished(int,QProcess::ExitStatus);
signals:
    void allDone(int); //we add a new signal

public slots:

};

#endif // WPSRUN_H
