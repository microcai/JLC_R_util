
#include "LLC_window.hpp"
#include <QMessageBox>
#include "qtcoro.hpp"
#include "R_gen.hpp"
#include "z_gen.hpp"

#include "LLC_window.moc"

LLC_window::LLC_window(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setupUi(this);
}

qtcoro::awaitable<void> LLC_window::do_calc()
{
    auto Lm = this->Lm->value() * 0.0000001;
    auto Lr = this->Lr->value() * 0.0000001;
    auto C = this->Cf->value() * 0.0000001;
    auto R = this->Rload->value();

    auto freq1 = 1.0 / (2 * std::numbers::pi * sqrt( Lm * C));
    auto freq2 = 1.0 / (2 * std::numbers::pi * sqrt( Lr * C));

    auto freq = freq1 + (1.0/((R + 1)*(R + 1)))*(freq2-freq1);

    this->Resonant_frequency->setText(tr("%1").arg(freq/1000));
    this->Resonant_frequency1->setText(tr("%1").arg(freq1/1000));
    this->Resonant_frequency2->setText(tr("%1").arg(freq2/1000));

    begin_calc->setEnabled(true);

    co_return;
}

void LLC_window::on_begin_calc_clicked()
{
    begin_calc->setEnabled(false);
    qtcoro::coro_start(do_calc());
}

void LLC_window::check_caculatable()
{
    
}

