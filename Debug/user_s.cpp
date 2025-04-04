#include "user_s.h"
#include "ui_user_s.h"
#include "DBHelper.h"
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QtCharts>
User_s::User_s(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::User_s)
{
    ui->setupUi(this);
    connect(ui->pbt2,&QPushButton::clicked,[=](){
        //发一个信号过去
        emit this->back_user_s();
    });
}

User_s::~User_s()
{
    delete ui;
}


void User_s::on_pbt1_clicked()
{
    DBHelper *helper=DBHelper::getInstance();
    helper->creatConn();
    QSqlQuery query;
    int cnt = 0;
    //清空verticalLayout布局内的所有元素
    QLayoutItem *child;
    while ((child = ui->verticalLayout->takeAt(0)) != 0)
    {
        //setParent为NULL，防止删除之后界面不消失
        if(child->widget())
            child->widget()->setParent(NULL);
        delete child;
    }
    //清除textBrowser内的文本
    ui->textBrowser->clear();
    QPieSeries *series = new QPieSeries();
    QPieSlice* slice;
    QString time = ui->lineEdit2->text() + '-' +
            (ui->lineEdit3->text().toInt()<10 ? '0'+ui->lineEdit3->text():ui->lineEdit3->text()) + '-' +
            (ui->lineEdit4->text().toInt()<10 ? '0'+ui->lineEdit4->text():ui->lineEdit4->text());

    query.prepare("select * from tb_tel where usside = :us_temp and starttime > :times and starttime < :timee;");
    query.bindValue(":us_temp",ui->lineEdit1->text());
    query.bindValue(":times",time);
    query.bindValue(":timee",time+"T23:59:59");
    query.exec();
    while(query.next())
    {
        series->append(query.value(1).toString(), query.value(3).toInt());
        slice = series->slices().at(cnt);
        ui->textBrowser->append(QString::number(cnt+1)+"、当日"+query.value(2).toString().mid(11)+"和"+slice->label()+
              "通信"+QString::number(slice->value())+"分钟");
        cnt++;
    }

    query.prepare("select * from tb_tel where otherside = :other_temp and starttime > :times and starttime < :timee;");
    query.bindValue(":other_temp",ui->lineEdit1->text());
    query.bindValue(":times",time);
    query.bindValue(":timee",time+"T23:59:59");
    query.exec();
    while(query.next())
    {
        series->append(query.value(0).toString(), query.value(3).toInt());
        slice = series->slices().at(cnt);
        ui->textBrowser->append(QString::number(cnt+1)+"、当日"+query.value(2).toString().mid(11)+"和"+slice->label()+
              "通讯"+QString::number(slice->value())+"分钟");
        cnt++;
    }

    series->setHoleSize(0);//饼图中间是否需要填洞
    series->setPieSize(1);//设置大小：这里的大小就是占据整个view的大小
    //series->setLabelsVisible();
    QChart* chart = new QChart();
    chart->addSeries(series);// 将创建好的饼图对象添加进QChart
    chart->setTitle("联络者、联络时间及占比");
    chart->setAnimationOptions(QChart::AllAnimations);//设置动画效果
    chart->legend()->setAlignment(Qt::AlignRight);

    // 获取饼图中某一个切片（在绘制圆环图Demo中是通过appent函数获取，这里换一种方式）
    for(int i=0;i<series->count();i++)//处理每个切片对应的维度
    {
        slice = series->slices().at(i);
        slice->setLabel(QString::asprintf("%s:%.0fmins,%.1f%%",slice->label().toStdString().data(),slice->value(),slice->percentage()*100));//不能放在前面，延时
    }

    // 创建QChartView 对象
    QChartView *chartView;
    // QChartView 对象载入饼状图
    chartView = new QChartView(chart);
    // 显示图标
    ui->verticalLayout->insertWidget(0,chartView);
    helper->destroyConn();
}
