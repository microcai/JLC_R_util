

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

