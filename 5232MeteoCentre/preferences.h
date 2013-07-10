#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include "QFile"
#include "QtXml"

class Preferences : public QObject
{
    Q_OBJECT
public:
    explicit Preferences(QObject *parent = 0);
    bool writeXml(const QString &fileName);
    bool readXml(const QString &fileName);
    QString work_path;
    QString rzd_path;
    QString rzd_filename;
    QString site_path;

signals:

public slots:
private:



};

#endif // PREFERENCES_H
