
#pragma once

#include <QWidget>
#include "ui_I_Calc.h"
#include "qtcoro.hpp"

class I_Calc : public QWidget, Ui_I_Calc
{
    Q_OBJECT

public:
    explicit I_Calc(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

private:
    qtcoro::awaitable<void> do_calc();

public Q_SLOTS:
    void check_caculatable();
    void on_begin_calc_clicked();
};

