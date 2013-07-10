#include <QtCore/QCoreApplication>
#include "wpsrun.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    WPSRun run1(&app);
    if(!run1.initialize("preferences.xml"))
        QCoreApplication::exit(5);
    run1.Analize();
    QCoreApplication::exit(0);
}
