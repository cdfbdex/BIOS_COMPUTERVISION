#include "src/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MABE");
    QCoreApplication::setApplicationName("SIV");
    QCoreApplication::setOrganizationDomain("http://www.mabe.com.co/");

    QApplication app(argc, argv);
    MainWindow w;

    w.showMaximized();
    return app.exec();
}
