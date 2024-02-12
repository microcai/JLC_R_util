
#include "R_select_window.hpp"
#include "ui_R_select_window.h"
#include "awaitable.hpp"
#include "R_gen.hpp"
#include "z_gen.hpp"

#include "R_select_window.moc"

R_select_window::R_select_window(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setupUi(this);
    pic_show->setPixmap(QPixmap(":/images/res/dcdc.png").scaledToWidth(550));

    connect(fb_voltage, SIGNAL(editingFinished()), this, SLOT(check_caculatable()));
    connect(out_voltage, SIGNAL(editingFinished()), this, SLOT(check_caculatable()));
    connect(resistance_type, SIGNAL(currentIndexChanged(int)), this, SLOT(check_caculatable()));
    connect(output_accuracy, SIGNAL(currentIndexChanged(int)), this, SLOT(check_caculatable()));

    auto Validator = new QDoubleValidator(0.0, 9999.0, 5, this);

    Validator->setNotation(QDoubleValidator::StandardNotation);

    fb_voltage->setValidator(Validator);
    out_voltage->setValidator(Validator);
}

qtcoro::awaitable<void> R_select_window::do_calc()
{
    static const std::pair<double, double> accuracy_config[] = {
        {0.99, 1.01},
        {0.95, 1.05},
        {0.90, 1.10},
    };

    auto r_type = resistance_type->currentIndex();
    auto is_jlc = r_type == 2;
    auto Vout = out_voltage->text().toDouble();
    auto Vref = fb_voltage->text().toDouble();

    auto accuracy = accuracy_config[output_accuracy->currentIndex()];

    R_lib r{r_type};
    Zgen z;

    int row = 0;

    tableWidget->clearContents();

    std::pair<int, int> Zidx;

    try
    {
        for(;;)
        {
            Zidx = z();

            auto R1 = r(Zidx.first);
            auto R2 = r(Zidx.second);

            auto Vout_calculated = Vref * (static_cast<double>(R1) + static_cast<double>(R2)) / static_cast<double>(R2);
            auto Err = Vout_calculated / Vout - 1;

            if (((Vout_calculated / Vout) >= accuracy.first) && ( (Vout_calculated / Vout) <= accuracy.second ))
            {
                tableWidget->setRowCount(row+1);
                tableWidget->setItem(row, 0, new QTableWidgetItem(static_cast<QString>(R1)));
                tableWidget->setItem(row, 1, new QTableWidgetItem(static_cast<QString>(R2)));
                tableWidget->setItem(row, 2, new QTableWidgetItem(QString("%1").arg(Vout_calculated)));
                tableWidget->setItem(row, 3, new QTableWidgetItem(QString("%1%").arg(Err*100)));
                if (is_jlc)
                {
                    // R_lib
                    tableWidget->setItem(row, 4, new QTableWidgetItem(QString("%1 %2").arg(R1.jlc_part_number).arg(R2.jlc_part_number)));
                }

                row ++;
                
                co_await coro_delay_ms(10);
            }           
            
        }
    }catch(const std::out_of_range&)
    {
        // QMessageBox::warning(this, "d", QString("oom on (%1,%2)").arg(Zidx.first).arg(Zidx.second));
    }

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    
    
    begin_calc->setEnabled(true);

    co_return;
}

void R_select_window::on_begin_calc_clicked()
{
    begin_calc->setEnabled(false);
    start_coro(do_calc());
}

void R_select_window::check_caculatable()
{
    
}

