
#include <numbers>
#include <iostream>
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

    connect(this->freq_slide, SIGNAL(sliderMoved(int)), this, SLOT(freqSlided(int)));
}

qtcoro::awaitable<void> LLC_window::do_calc()
{
    auto Lm = this->Lm->value() * 0.0000001;
    auto Lr = this->Lr->value() * 0.0000001;
    auto C = this->Cf->value() * 0.0000001;
    auto R = this->Rload->value();

    freq1 = 1.0 / (2 * std::numbers::pi * sqrt( Lm * C));
    freq2 = 1.0 / (2 * std::numbers::pi * sqrt( Lr * C));

    freq = freq1 + (1.0/((R + 1)*(R + 1)))*(freq2-freq1);

    this->Resonant_frequency->setText(tr("%1").arg(freq/1000));
    this->Resonant_frequency1->setText(tr("%1").arg(freq1/1000));
    this->Resonant_frequency2->setText(tr("%1").arg(freq2/1000));

    auto R_C = 1.0 / (2.0*std::numbers::pi * C * freq );
    auto R_Lr = 2.0*std::numbers::pi * Lr * freq;

    auto R_Lm = 2.0*std::numbers::pi * freq *  (Lm - Lr) * R / ( R + Lm - Lr);

    auto zengyi = 2.0* (R_Lm) / (R_C + R_Lr + R_Lm);

    this->gain->setText(tr("%1").arg(zengyi));

    begin_calc->setEnabled(true);

    co_return;
}

void LLC_window::freqSlided(int pos)
{
    freq = freq1 + (freq2-freq1)* (pos) / freq_slide->maximum();
    this->Resonant_frequency->setText(tr("%1").arg(freq/1000));

    auto Lm = this->Lm->value() * 0.0000001;
    auto Lr = this->Lr->value() * 0.0000001;
    auto C = this->Cf->value() * 0.0000001;
    auto R = this->Rload->value();

    auto R_C = 1.0 / (2.0*std::numbers::pi * C * freq );
    auto R_Lr = 2.0*std::numbers::pi * Lr * freq;

    auto R_Lm = 2.0*std::numbers::pi * freq *  (Lm - Lr) * R / ( R + Lm - Lr);

    auto zengyi = 2.0* (R_Lm) / (R_C + R_Lr + R_Lm);

    this->gain->setText(tr("%1").arg(zengyi));    
}

void LLC_window::on_begin_calc_clicked()
{
    begin_calc->setEnabled(false);
    qtcoro::coro_start(do_calc());
}

void LLC_window::check_caculatable()
{
    
}

