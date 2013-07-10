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
    QString netcdf1;
    QString netcdf2;
    QString output;

signals:

public slots:
private:



};

#endif // PREFERENCES_H
