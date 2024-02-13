
#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QIcon ico(":/images/res/logo.png");
    app.setWindowIcon(ico);

    MainWindow mywin;

    mywin.setWindowIcon(ico);

    mywin.show();

    return app.exec();
}
