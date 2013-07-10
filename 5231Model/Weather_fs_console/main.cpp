#include <QtCore/QCoreApplication>
#include "QDebug"
#include "wpsrun_console.h"

/** Для записи qDebug() в файл**/
QString fName = QDateTime::currentDateTime().toString("hh:mm:ss_dd.MM.yyyy") + ".txt";

void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
    break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
    break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    }


    QFile outFile(fName);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{

    QCoreApplication app(argc, argv); //renamed the a to app
    qInstallMsgHandler(customMessageHandler); //Инсталируем Хендлер
    qDebug()<<"Start time: "<<QDateTime::currentDateTime().toString("hh:mm:ss_dd.MM.yyyy");
    wpsrun_console run1(&app);
    if(run1.Init())
        run1.Start();
    else
        return -1;
    return app.exec(); //and we run the application
}

