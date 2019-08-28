#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_Time_valueChanged(const QString &arg1);

    void on_Start_clicked();

    void on_Stop_clicked();

    void on_Pause_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
