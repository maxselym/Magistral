#ifndef WPSRUN_H
#define WPSRUN_H

#include <QObject>
#include <QDate>
#include "QDir"
#include "xml_namelist.h"
#include "QProcess"
#include "preferences.h"
#include "qlist.h"


const QString Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
class WPSRun : public QObject
{
    Q_OBJECT

public:
    explicit WPSRun(QObject *parent = 0);
    bool initialize(QString sPreferenceFile);
    void Analize();
    void readpress1(QString file);
    void readpress2(QString file);
    void ClearDir(const QString d);


private:
    QList<float> press1;
    QList<float> press2;
    Preferences* pref;
    xml_namelist* p_namelist_tool;
    QString wrfRoot;
    QString wpsRoot;
    QString gribFiles;
    QString outputRoot;
    QString namelistWps;
    int CoreNum;
    int n_proc;
    QProcess* proc;

public slots:

};

#endif // WPSRUN_H
