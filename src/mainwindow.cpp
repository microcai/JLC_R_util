

#include "mainwindow.hpp"
#include <QMessageBox>
#include "awaitable.hpp"
#include "R_gen.hpp"
#include "z_gen.hpp"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    setupUi(this);

    pic_show->setPixmap(QPixmap(":/images/res/dcdc.png").scaledToWidth(800));

    connect(fb_voltage, SIGNAL(editingFinished()), this, SLOT(check_caculatable()));
    connect(out_voltage, SIGNAL(editingFinished()), this, SLOT(check_caculatable()));
    connect(resistance_type, SIGNAL(currentIndexChanged()), this, SLOT(check_caculatable()));
    connect(output_accuracy, SIGNAL(currentIndexChanged()), this, SLOT(check_caculatable()));
}

MainWindow::~MainWindow(){}

qtcoro::awaitable<void> MainWindow::do_calc()
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
                tableWidget->setItem(row, 3, new QTableWidgetItem(QString("%1%%").arg(Err*100)));
                if (is_jlc)
                {
                    // R_lib
                    tableWidget->setItem(row, 4, new QTableWidgetItem(QString("%1 %2").arg(R1.jlc_part_number).arg(R2.jlc_part_number)));
                }

                row ++;
            }           
            
        }
    }catch(const std::out_of_range&)
    {
        // QMessageBox::warning(this, "d", QString("oom on (%1,%2)").arg(Zidx.first).arg(Zidx.second));
    }

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    co_return;
}

void MainWindow::on_begin_calc_clicked()
{
    start_coro(do_calc());
}

void MainWindow::check_caculatable()
{
    
}