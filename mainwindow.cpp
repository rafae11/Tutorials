#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QtMath>

#include <QValueAxis>
#include <QLineSeries>
#include <QSplineSeries>
#include <QCategoryAxis>
#include <QScatterSeries>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_SettingsFile = "C:/Temp/config.ini";

    m_IncomeModel = new QStringListModel();
    m_ExpensesModel = new QStringListModel();
    m_CashFlowModel = new QStringListModel();
    m_CashOnCashModel = new QStringListModel();

    ui->IncomeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->IncomeView->setModel(m_IncomeModel);

    ui->ExpensesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->ExpensesView->setModel(m_ExpensesModel);

    LoadSettings();

    CalculateTotalIncome();
    CalculateTotalExpenses();
    CalculateCashFLow();
    CashOnCashReturn();

    DisableActions();

    CalculateMontlyPayment();

    chart  = new QChart();
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartlayout->addWidget(chartView);

    ReDraw();

}

MainWindow::~MainWindow()
{
    SaveSettings();

    delete ui;
}

void MainWindow::on_AddIncome_clicked()
{
    AppendIncomeList();
    CalculateTotalIncome();
    CalculateCashFLow();
    CashOnCashReturn();
}

void MainWindow::on_AddExpenses_clicked()
{
    AppendExpensesList();
    CalculateTotalExpenses();
    CalculateCashFLow();
    CashOnCashReturn();
}

void MainWindow::on_DeleteIncome_clicked()
{
    QModelIndex index = ui->IncomeView->currentIndex();
    m_IncomeModel->removeRows(index.row(),1);

    CalculateTotalIncome();

    ui->IncomeView->setModel(m_IncomeModel);

    CalculateCashFLow();
    CashOnCashReturn();
}

void MainWindow::on_DeleteExpenses_clicked()
{
    QModelIndex index = ui->ExpensesView->currentIndex();
    m_ExpensesModel->removeRows(index.row(),1);

   CalculateTotalExpenses();

   ui->ExpensesView->setModel(m_ExpensesModel);

   CalculateCashFLow();
   CashOnCashReturn();
}

void MainWindow::AppendIncomeList()
{
    QString Text(ui->Income->text());
    Text.append(" ").append(QString::number(ui->IncomePrice->value()));

    m_IncomeList << Text;
    m_IncomeModel->setStringList(m_IncomeList);
}

void MainWindow::AppendExpensesList()
{
    QString Text(ui->Expenses->text());
    Text.append(" ").append(QString::number(ui->ExpensesPrice->value()));

    m_ExpensesList << Text;
    m_ExpensesModel->setStringList(m_ExpensesList);
}

void MainWindow::CalculateTotalIncome()
{
    m_IncomeList = m_IncomeModel->stringList();

    m_IncomeVector.clear();

    if(!m_IncomeList.isEmpty())
    {
        foreach(QString item, m_IncomeList)
        {
            QStringList items = item.split(" ");

            if(items.size() > 1)
            {
                m_IncomeVector.push_back(items[1].toDouble());
            }
        }

        ui->IncomeTotal->setText(QString::number(std::accumulate(m_IncomeVector.begin(),m_IncomeVector.end(),0.0)));
    }

}

void MainWindow::CalculateTotalExpenses()
{
    m_ExpensesList = m_ExpensesModel->stringList();

    m_ExpensesVector.clear();

    if(!m_ExpensesList.isEmpty())
    {
        foreach(QString item, m_ExpensesList)
        {

            QStringList items = item.split(" ");
            if(items.size() > 1)
            {
                m_ExpensesVector.push_back(items[1].toDouble());
            }
        }

        ui->ExpensesTotal->setText(QString::number(std::accumulate(m_ExpensesVector.begin(),m_ExpensesVector.end(),0.0)));
    }

}

void MainWindow::CalculateCashFLow()
{
    double CashFlow = ui->IncomeTotal->text().toDouble() - ui->ExpensesTotal->text().toDouble();

    QStringList StringList;
    StringList << "Monthly Cash Flow " << QString::number(CashFlow);
    StringList << "Annual Cash Flow " << QString::number(CashFlow * 12);
    m_CashFlowModel->setStringList(StringList);

    ui->CashFlow->setModel(m_CashFlowModel);
}

void MainWindow::CashOnCashReturn()
{
    QAbstractItemModel      *m_model = ui->CashFlow->model();

    QModelIndex index = m_model->index(1,0);
    double result = (index.data().toDouble() / ui->Investment->value()) * 100.0;

    QStringList list;
    list << "Monthly " << QString::number(result).append("%");

    index = m_model->index(3,0);
    result = (index.data().toDouble() / ui->Investment->value()) * 100.0;

    list << "Annual " << QString::number(result).append("%");

    m_CashOnCashModel->setStringList(list);
    ui->CashOnCash->setModel(m_CashOnCashModel);

}

void MainWindow::LoadSettings()
{
    QSettings Settings(m_SettingsFile, QSettings::IniFormat);

    double IncomePrice = Settings.value("IncomePrice", "").toDouble();
    ui->IncomePrice->setValue(IncomePrice);

    double ExpensesPrice = Settings.value("ExpensesPrice","").toDouble();
    ui->ExpensesPrice->setValue(ExpensesPrice);

    QString IncomeId = Settings.value("IncomeId","").toString();
    ui->Income->setText(IncomeId);

    QString ExpensesId = Settings.value("ExpensesId","").toString();
    ui->Expenses->setText(ExpensesId);

    double InitialInvestment = Settings.value("InitialInvestment","").toDouble();
    ui->Investment->setValue(InitialInvestment);

    QStringList IncomeList = Settings.value("IncomeModel","").toStringList();
    m_IncomeModel->setStringList(IncomeList);

    QStringList ExpensesList = Settings.value("ExpensesModel","").toStringList();
    m_ExpensesModel->setStringList(ExpensesList);

    restoreGeometry(Settings.value("mainWindowGeometry").toByteArray());
    restoreState(Settings.value("mainWindowState").toByteArray());
}

void MainWindow::SaveSettings()
{
    QSettings Settings(m_SettingsFile, QSettings::IniFormat);

    double IncomePrice = ui->IncomePrice->value();
    double ExpensesPrice = ui->ExpensesPrice->value();
    double InitialInvestment = ui->Investment->value();
    QString IncomeId = ui->Income->text();
    QString ExpensesId = ui->Expenses->text();
    QStringList IncomeModel = m_IncomeModel->stringList();
    QStringList ExpensesModel = m_ExpensesModel->stringList();

    Settings.setValue("IncomeModel",IncomeModel);
    Settings.setValue("ExpensesModel",ExpensesModel);
    Settings.setValue("InitialInvestment",InitialInvestment);
    Settings.setValue("IncomeId", IncomeId);
    Settings.setValue("ExpensesId", ExpensesId);
    Settings.setValue("IncomePrice", IncomePrice);
    Settings.setValue("ExpensesPrice", ExpensesPrice);

    Settings.setValue("mainWindowGeometry", saveGeometry());
    Settings.setValue("mainWindowState", saveState());
}

void MainWindow::on_Investment_editingFinished()
{
    CashOnCashReturn();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionCashFlow_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
    DisableActions();
}

void MainWindow::on_actionMortgage_Calculator_triggered()
{
   ui->stackedWidget->setCurrentIndex(1);

   DisableActions();
}

void MainWindow::DisableActions()
{
    switch(ui->stackedWidget->currentIndex())
    {
        case 0:
        {
            ui->actionCashFlow->setEnabled(false);
            ui->actionMortgage_Calculator->setEnabled(true);
            ui->stackedWidget->setCurrentIndex(0);
        }
        break;

        case 1:
        {
            ui->actionCashFlow->setEnabled(true);
            ui->actionMortgage_Calculator->setEnabled(false);
            ui->stackedWidget->setCurrentIndex(1);
        }
        break;
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox Msgbox;
    Msgbox.setText("Investment Calculator Version 1.0");
    Msgbox.exec();
}

void MainWindow::CalculateMontlyPayment()
{
    //ui->InterestRateSlider->setValue(12);
    //ui->YearsSlider->setValue(20);
    //ui->PrincipalSlider->setValue(100000);

    double MonthlyInterest = (ui->InterestSpinBox->value() / 100.0) / 12.0;
    double MonthsInYears = ui->YearsSpinBox->value() * 12.0;
    double Numerator = MonthlyInterest * qPow(1 + MonthlyInterest, MonthsInYears );
    double Denominator = qPow(1 + MonthlyInterest, MonthsInYears ) - 1;
    double monthlypayment = ui->PrincipalSpinBox->value() * (Numerator / Denominator);

    ui->MonthlyAmount->setText(QString::number(monthlypayment + MonthlyInterest,'g',5));
    ui->PrincipalAmount->setText(QString::number(ui->PrincipalSpinBox->value(),'g',10));
    ui->InterestRate->setText(QString::number(ui->InterestSpinBox->value()));
    ui->YearsToPay->setText(QString::number(ui->YearsSpinBox->value()));


}

void MainWindow::ReDraw()
{
    m_Equity.clear();
    m_LoanAmount.clear();
    m_TotalInterest.clear();

    chart  = new QChart();
    chartView->setChart(chart);

    QLineSeries* series = new QLineSeries();

    QScatterSeries* series0 = new QScatterSeries();
    series0->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series0->setMarkerSize(10.0);


    QLineSeries* series1 = new QLineSeries();

    QScatterSeries* series2 = new QScatterSeries();
    series2->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series2->setMarkerSize(10.0);


    double equity = ui->InitialDeposit->text().toLong();
    double rolling = ui->PrincipalAmount->text().toLong();
    double interest = (ui->InterestSpinBox->value() / 100.0) / 12.0;
    double monthlyamount = ui->MonthlyAmount->text().toDouble();

    for(int i=0; i < ui->YearsToPay->text().toLong(); i++)
    {
        double yearlysum = 0;

        for(int j=0; j < 12; j++)
        {
            double monthlyinterest =  rolling * interest;
            rolling += monthlyinterest;

            double balance = rolling - monthlyamount;

            yearlysum += (rolling - balance);
            m_TotalInterest.push_back(monthlyinterest);
        }

        rolling -= yearlysum;
        equity += yearlysum;

        m_Equity.push_back(equity);
        m_LoanAmount.push_back(rolling);
        series->append(i, rolling);
        series0->append(i,rolling);
        series1->append(i,equity);
        series2->append(i,equity);
    }


    chart->legend()->hide();
    chart->setTitle("Multiaxis chart example");

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Equity and Loan Balance");
    axisY->setRange(0,ui->PrincipalAmount->text().toDouble() + std::accumulate(m_TotalInterest.begin(),m_TotalInterest.end(),0.0));
    axisY->setTickCount(10);
    axisY->applyNiceNumbers();

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Years");
    axisX->setRange(0,ui->YearsToPay->text().toLong() + 1);
    axisX->setTickCount(10);
    axisX->applyNiceNumbers();

    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);

    chart->addSeries(series);
    chart->addSeries(series0);
    chart->addSeries(series1);
    chart->addSeries(series2);

    series->attachAxis(axisX);
    series0->attachAxis(axisX);
    series->attachAxis(axisY);
    series0->attachAxis(axisY);

    series1->attachAxis(axisX);
    series2->attachAxis(axisX);
    series1->attachAxis(axisY);
    series2->attachAxis(axisY);

}

void MainWindow::on_PrincipalSpinBox_editingFinished()
{
    CalculateMontlyPayment();
    ReDraw();

}

void MainWindow::on_InterestSpinBox_editingFinished()
{
    CalculateMontlyPayment();
    ReDraw();

}

void MainWindow::on_YearsSpinBox_editingFinished()
{
    CalculateMontlyPayment();
    ReDraw();

}

void MainWindow::on_PrincipalSpinBox_valueChanged(const QString &arg1)
{
    CalculateMontlyPayment();
    ReDraw();

}

void MainWindow::on_InterestSpinBox_valueChanged(const QString &arg1)
{
    CalculateMontlyPayment();
    ReDraw();

}

void MainWindow::on_YearsSpinBox_valueChanged(const QString &arg1)
{
    CalculateMontlyPayment();
    ReDraw();

}
