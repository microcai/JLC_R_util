
#include "I_Calc.hpp"
#include <QMessageBox>
#include "awaitable.hpp"
#include "R_gen.hpp"
#include "z_gen.hpp"

#include "I_Calc.moc"

I_Calc::I_Calc(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setupUi(this);
}

qtcoro::awaitable<void> I_Calc::do_calc()
{
    auto Vout = this->Vout->value();
    auto Vin = this->Vin->value();
    auto Fsw = this->Fsw->value();
    auto Iout = this->Iout->value();
    auto Kind = this->Kind->value();

    auto Lmin =  Vout * (Vin - Vout) / (Vin * Kind * Iout * Fsw);

    auto row = 0;

    result->setText(QString("%1uH").arg(static_cast<int>(Lmin*1000000)));

    begin_calc->setEnabled(true);

    co_return;
}

void I_Calc::on_begin_calc_clicked()
{
    begin_calc->setEnabled(false);
    start_coro(do_calc());
}

void I_Calc::check_caculatable()
{
    
}

