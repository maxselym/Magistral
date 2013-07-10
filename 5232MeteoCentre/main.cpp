#include <QtCore/QCoreApplication>
#include "wpsrun.h"
#include "QDebug"

/** Для записи qDebug() в файл**/
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

    QFile outFile("debuglog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
 //   qInstallMsgHandler(customMessageHandler); //Инсталируем Хендлер

    /** Русский язык UTF*/
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    qDebug()<<"Start time: "<<QDateTime::currentDateTime().toString("hh:mm:ss_dd.MM.yyyy");
    WPSRun run1(&app);
    run1.initialize("preferences.xml");
    run1.StartProc();
    return app.exec();
}
