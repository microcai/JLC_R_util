
#pragma once

#include <QWidget>
#include "ui_R_select_window.h"
#include "awaitable.hpp"

class R_select_window : public QWidget, Ui_R_select_window
{
    Q_OBJECT

public:
    explicit R_select_window(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

private:
    qtcoro::awaitable<void> do_calc();

public Q_SLOTS:
    void check_caculatable();
    void on_begin_calc_clicked();
};

