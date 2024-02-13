
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
    co_return;
}

void I_Calc::on_begin_calc_clicked()
{

}

void I_Calc::check_caculatable()
{
    
}

