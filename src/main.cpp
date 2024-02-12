
#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow mywin;

    mywin.show();

    return app.exec();
}
