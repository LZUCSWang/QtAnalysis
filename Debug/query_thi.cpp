#include "query_thi.h"
#include "ui_query_thi.h"
#include "DBHelper.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
Query_thi::Query_thi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Query_thi)
{
    ui->setupUi(this);
    connect(ui->pbt1,&QPushButton::clicked,[=](){
        //发一个信号过去
        emit this->back_query_thi();
    });
}

Query_thi::~Query_thi()
{
    delete ui;
}

void Query_thi::tbshow(QString timestart, QString timeend)//在表格中展示数据库中表的内容
{
    DBHelper *helper=DBHelper::getInstance();
    helper->creatConn();
    QSqlQuery query;
    int cnt = 0;
    query.prepare("select * from tb_tel where starttime > :times and starttime < :timee order by duration desc;");
    query.bindValue(":times",timestart);
    query.bindValue(":timee",timeend+"T23:59:59");
    query.exec();
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(40);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"Us","Other","Time","Minutes"});
    ui->tableWidget->setColumnWidth(2,200);
    while(query.next())
    {

        QString Us = query.value(0).toString();
        QString Other = query.value(1).toString();
        QString Time = query.value(2).toString();
        int Minutes = query.value(3).toInt();
        ui->tableWidget->setItem(cnt,0,new QTableWidgetItem(Us));
        ui->tableWidget->setItem(cnt,1,new QTableWidgetItem(Other));
        ui->tableWidget->setItem(cnt,2,new QTableWidgetItem(Time));
        ui->tableWidget->setItem(cnt,3,new QTableWidgetItem(QString::number(Minutes)));
        cnt++;
    }
    helper->destroyConn();
}
