#include "query_fir.h"
#include "ui_query_fir.h"
#include "DBHelper.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
Query_fir::Query_fir(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Query_fir)
{
    ui->setupUi(this);
    connect(ui->pbt1,&QPushButton::clicked,[=](){
        //发一个信号过去
        emit this->back_query_fir();
    });
}

Query_fir::~Query_fir()
{
    delete ui;
}

void Query_fir::tbshow(QString us, QString other)//在表格中展示数据库中表的内容
{
    DBHelper *helper=DBHelper::getInstance();
    helper->creatConn();
    QSqlQuery query;
    int cnt = 0;
    query.prepare("select * from tb_tel where usside = :us_temp and otherside = :other_temp;");
    query.bindValue(":us_temp",us);
    query.bindValue(":other_temp",other);
    query.exec();
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(40);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({"Time","Minutes"});
    ui->tableWidget->setColumnWidth(0,200);
    while(query.next())
    {
        QString Time = query.value(2).toString();
        int Minutes = query.value(3).toInt();
        ui->tableWidget->setItem(cnt,0,new QTableWidgetItem(Time));
        ui->tableWidget->setItem(cnt,1,new QTableWidgetItem(QString::number(Minutes)));
        cnt++;
    }
    helper->destroyConn();
}
