
#include <numbers>
#include <iostream>
#include "RCD_window.hpp"
#include <QMessageBox>
#include "qtcoro.hpp"
#include "R_gen.hpp"
#include "z_gen.hpp"

#include "RCD_window.moc"

RCD_window::RCD_window(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setupUi(this);
    connect(this->duty_slide, SIGNAL(sliderMoved(int)), this, SLOT(DutySlided(int)));
    DutySlided(this->duty_slide->value());
}

void RCD_window::DutySlided(int pos)
{
    if (pos > 50)
    {
        this->duty_slide->setSliderPosition(50);
        pos = 50;
    }

    this->label_duty->setText(tr("%1%").arg(pos));

    qtcoro::coro_start(do_calc());
}


void RCD_window::on_begin_calc_clicked()
{
    begin_calc->setEnabled(false);
    qtcoro::coro_start(do_calc());
}

void RCD_window::check_caculatable()
{
    
}

qtcoro::awaitable<void> RCD_window::do_calc()
{
    begin_calc->setEnabled(true);

    // 根据占空比，计算反射电压


    co_return;
}
