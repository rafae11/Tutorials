#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Time_valueChanged(const QString &arg1)
{

}

void MainWindow::on_Start_clicked()
{

}

void MainWindow::on_Stop_clicked()
{

}

void MainWindow::on_Pause_clicked()
{

}
