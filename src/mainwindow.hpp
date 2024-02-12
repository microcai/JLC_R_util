
#pragma once

#include <QObject>
#include <QMainWindow>
#include "awaitable.hpp"

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MainWindow();

private:
    qtcoro::awaitable<void> do_calc();

public Q_SLOTS:
    void check_caculatable();
    void on_begin_calc_clicked();
};

#include "mainwindow.moc"
