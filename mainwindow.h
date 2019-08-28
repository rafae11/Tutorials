#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QSettings>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

#include <QGridLayout>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_AddIncome_clicked();
    void on_AddExpenses_clicked();

    void on_DeleteIncome_clicked();
    void on_DeleteExpenses_clicked();

    void AppendIncomeList();
    void AppendExpensesList();

    void CalculateTotalIncome();
    void CalculateTotalExpenses();

    void CalculateCashFLow();
    void CashOnCashReturn();

    void LoadSettings();
    void SaveSettings();

    void DisableActions();

    void on_Investment_editingFinished();

    void on_actionExit_triggered();

    void on_actionCashFlow_triggered();

    void on_actionMortgage_Calculator_triggered();

    void on_actionAbout_triggered();

    void CalculateMontlyPayment();
    void ReDraw();

    void on_PrincipalSpinBox_editingFinished();

    void on_InterestSpinBox_editingFinished();

    void on_YearsSpinBox_editingFinished();

    void on_PrincipalSpinBox_valueChanged(const QString &arg1);

    void on_InterestSpinBox_valueChanged(const QString &arg1);

    void on_YearsSpinBox_valueChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QString                 m_SettingsFile;

    QStringListModel        *m_CashFlowModel;
    QStringListModel        *m_IncomeModel;
    QStringListModel        *m_ExpensesModel;
    QStringListModel        *m_CashOnCashModel;

    QStringList             m_ExpensesList;
    QStringList             m_IncomeList;

    std::vector<double>     m_IncomeVector;
    std::vector<double>     m_ExpensesVector;
    std::vector<double>     m_Equity;
    std::vector<double>     m_LoanAmount;
    std::vector<double>     m_TotalInterest;

    QGridLayout* gridLayot;
    QChart *chart;
    QChartView *chartView;

};

#endif // MAINWINDOW_H
