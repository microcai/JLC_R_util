
#pragma once

#include <QWidget>
#include "ui_LLC_window.h"
#include "qtcoro.hpp"

class LLC_window : public QWidget, Ui_LLC_window
{
    Q_OBJECT

public:
    explicit LLC_window(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

private:
    qtcoro::awaitable<void> do_calc();

public Q_SLOTS:
    void check_caculatable();
    void on_begin_calc_clicked();
    void freqSlided(int);

private:
    qreal freq1, freq2, freq;
};

