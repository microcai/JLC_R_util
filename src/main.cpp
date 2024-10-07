
#include <QApplication>
#include "mainwindow.hpp"
#include <QDebug>

static const char desktop_file_content[] = R"ss([Desktop Entry]
Categories=Development;Electronics;
Comment=免费、强大、易用的PCB设计辅助工具
Icon=edatool
Name=JLC EDA Tool
Type=Application
StartupWMClass=edatool
)ss";

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    if (app.platformName() == "wayland")
    {
        // 解决 wayland 下 没 icon 的问题
        QFile iconfile(QDir::homePath() + "/.local/share/icons/edatool.png");
        QFile resiconfile(":/images/res/logo.png");
        resiconfile.open(QIODeviceBase::ReadOnly);
        iconfile.open(QIODeviceBase::ReadWrite);

        iconfile.write(resiconfile.readAll());
        resiconfile.close(); iconfile.close();

        QFile desktopfile(QDir::homePath() + "/.local/share/applications/edatool.desktop");
        if (desktopfile.open(QIODeviceBase::ReadWrite))
        {
            desktopfile.write(desktop_file_content, sizeof (desktop_file_content) - 1);
            desktopfile.write(QString("Exec=%1\n").arg(app.applicationFilePath()).toUtf8());
        }
        desktopfile.close();
    }

    QIcon ico(":/images/res/logo.png");
    app.setWindowIcon(ico);
    app.setDesktopFileName("edatool.desktop");

    MainWindow mywin;

    mywin.setWindowIcon(ico);

    mywin.show();

    return app.exec();
}
