
#pragma once

#include <QWidget>
#include "ui_RCD_window.h"
#include "qtcoro.hpp"

class RCD_window : public QWidget, Ui_RCD_window
{
    Q_OBJECT

public:
    explicit RCD_window(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

private:
    qtcoro::awaitable<void> do_calc();

public Q_SLOTS:
    void check_caculatable();
    void on_begin_calc_clicked();
    void DutySlided(int);

private:
    qreal freq1, freq2, freq;
};

