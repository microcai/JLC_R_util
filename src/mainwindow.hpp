
#pragma once

#include <QObject>
#include <QMainWindow>
#include "awaitable.hpp"

#include "ui_mainwindow.h"
#include "mainwindow.moc"

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MainWindow();

public Q_SLOTS:
    void on_action_About_triggered();

    void on_action_About_Qt_triggered();
};

