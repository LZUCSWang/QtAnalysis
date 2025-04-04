#include "query_sec.h"
#include "ui_query_sec.h"
#include "DBHelper.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
Query_sec::Query_sec(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Query_sec)
{
    ui->setupUi(this);
    connect(ui->pbt1,&QPushButton::clicked,[=](){
        //发一个信号过去
        emit this->back_query_sec();
    });
}

Query_sec::~Query_sec()
{
    delete ui;
}

void Query_sec::tbshow(QString destine)//在表格中展示数据库中表的内容
{
    DBHelper *helper=DBHelper::getInstance();
    helper->creatConn();
    QSqlQuery query;
    int cnt = 0;
    QString us = NULL;
    QString other = NULL;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(40);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels({"Name","Time","Minutes"});
    ui->tableWidget->setColumnWidth(1,200);

    query.prepare("select * from tb_tel where (usside = :us_temp) order by duration desc;");
    query.bindValue(":us_temp",destine);
    query.exec();
    while(query.next())
    {
        QString Name = query.value(1).toString();
        QString Time = query.value(2).toString();
        int Minutes = query.value(3).toInt();
        ui->tableWidget->setItem(cnt,0,new QTableWidgetItem(Name));
        ui->tableWidget->setItem(cnt,1,new QTableWidgetItem(Time));
        ui->tableWidget->setItem(cnt,2,new QTableWidgetItem(QString::number(Minutes)));
        cnt++;
    }

    query.prepare("select * from tb_tel where otherside = :other_temp order by duration desc;");
    query.bindValue(":other_temp",destine);
    query.exec();
    while(query.next())
    {
        QString Name = query.value(0).toString();
        QString Time = query.value(2).toString();
        int Minutes = query.value(3).toInt();
        ui->tableWidget->setItem(cnt,0,new QTableWidgetItem(Name));
        ui->tableWidget->setItem(cnt,1,new QTableWidgetItem(Time));
        ui->tableWidget->setItem(cnt,2,new QTableWidgetItem(QString::number(Minutes)));
        cnt++;
    }
    helper->destroyConn();
}
