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

void MainWindow::on_telWidget_clicked()
{
    telWidget *telwidget=new telWidget();
    telwidget->show();
}

void MainWindow::on_payWidget_clicked()
{
    payWidget *paywidget=new  payWidget();
    paywidget->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    this->close();
}
