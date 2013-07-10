#include <QtCore/QCoreApplication>
#include "meteomessage.h"

int main(int argc, char *argv[])
{
    /** Запускаем основной поток приложения*/
    QCoreApplication app(argc, argv);
    /** Запуск основного класса и передача ему управления*/
    MeteoMessage run1(&app);
    /** Соединяемся с базой данных*/
    run1.connectDB();
    /** Получить список станций*/
    QList<QString> str = run1.GetStations();
    /** Перечисляем весь список станций*/
    for(int i=0;i<str.count();i++)
    {
        int n = run1.GetMeteo(str[i]); /**<  Получить номер станции*/
        /** Если не ноль, то пишем предупреждение.
         * Если ноль, то ничего не пишем.*/
        if(n!=0)
        {
            /** Получить предупреждение из базы по индентификатору*/
            QString rep = run1.GetReport(n);
            run1.writeXml("report.xml",str[i],rep); /** Записать предупреждение*/
        }

    }

    return 0;
}
