#include "query.h"
#include "ui_query.h"
#include "DBHelper.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

Query::Query(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Query)
{
    ui->setupUi(this);
    connect(ui->pbt4,&QPushButton::clicked,[=](){
        //发一个信号过去
        emit this->back_query();
    });


    this->query_fir = new Query_fir;//实例化查询页面
    connect(ui->pbt1,&QPushButton::clicked,[=](){
        //所谓的切换窗口，其实就是把主窗口隐藏
        //第二个页面显示
        this->hide();//隐藏主菜单
        this->query_fir->show();//显示第二个窗口
    });
    connect(this->query_fir,&Query_fir::back_query_fir,[=](){
        this->show();
        this->query_fir->hide();
    });


    this->query_sec = new Query_sec;//实例化查询页面
    connect(ui->pbt2,&QPushButton::clicked,[=](){
        //所谓的切换窗口，其实就是把主窗口隐藏
        //第二个页面显示
        this->hide();//隐藏主菜单
        this->query_sec->show();//显示第二个窗口
    });
    connect(this->query_sec,&Query_sec::back_query_sec,[=](){
        this->show();
        this->query_sec->hide();
    });


    this->query_thi = new Query_thi;//实例化查询页面
    connect(ui->pbt3,&QPushButton::clicked,[=](){
        //所谓的切换窗口，其实就是把主窗口隐藏
        //第二个页面显示
        this->hide();//隐藏主菜单
        this->query_thi->show();//显示第二个窗口
    });
    connect(this->query_thi,&Query_thi::back_query_thi,[=](){
        this->show();
        this->query_thi->hide();
    });
}

Query::~Query()
{
    delete ui;
}

void Query::on_pbt1_clicked()
{
    this->query_fir->tbshow(ui->lineEdit1->text(),ui->lineEdit2->text());
}

void Query::on_pbt2_clicked()
{
    this->query_sec->tbshow(ui->lineEdit3->text());
}

void Query::on_pbt3_clicked()
{
    QString timestart = ui->lineEdit4->text() + '-' +
            (ui->lineEdit5->text().toInt()<10 ? '0'+ui->lineEdit5->text():ui->lineEdit5->text()) + '-' +
            (ui->lineEdit6->text().toInt()<10 ? '0'+ui->lineEdit6->text():ui->lineEdit6->text());
    QString timeend = ui->lineEdit7->text() + '-' +
            (ui->lineEdit8->text().toInt()<10 ? '0'+ui->lineEdit8->text():ui->lineEdit8->text()) + '-' +
            (ui->lineEdit9->text().toInt()<10 ? '0'+ui->lineEdit9->text():ui->lineEdit9->text());
    this->query_thi->tbshow(timestart, timeend);
}
