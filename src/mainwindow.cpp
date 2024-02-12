

#include "mainwindow.hpp"
#include <QMessageBox>
#include "awaitable.hpp"

#include "mainwindow.moc"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    setupUi(this);
}

MainWindow::~MainWindow(){}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::about(this, "关于..", "本工具 DR 出品。");
}

void MainWindow::on_action_About_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}
