#include "user_i.h"
#include "ui_user_i.h"
#include "DBHelper.h"
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QtCharts>
User_i::User_i(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::User_i)
{
    ui->setupUi(this);
    connect(ui->pbt2,&QPushButton::clicked,[=](){
        //发一个信号过去
        emit this->back_user_i();
    });
}

User_i::~User_i()
{
    delete ui;
}

void User_i::on_pbt1_clicked()
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
    QBarSeries *series = new QBarSeries();
    QBarSet *sets = NULL;
    //QStringList categories;

    QString times = ui->lineEdit2->text() + '-' +
            (ui->lineEdit3->text().toInt()<10 ? '0'+ui->lineEdit3->text():ui->lineEdit3->text()) + '-' +
            (ui->lineEdit4->text().toInt()<10 ? '0'+ui->lineEdit4->text():ui->lineEdit4->text());
    QString timee = ui->lineEdit5->text() + '-' +
            (ui->lineEdit6->text().toInt()<10 ? '0'+ui->lineEdit6->text():ui->lineEdit6->text()) + '-' +
            (ui->lineEdit7->text().toInt()<10 ? '0'+ui->lineEdit7->text():ui->lineEdit7->text());

    query.prepare("select * from tb_tel where usside = :us_temp and starttime > :times and starttime < :timee;");
    query.bindValue(":us_temp",ui->lineEdit1->text());
    query.bindValue(":times",times);
    query.bindValue(":timee",timee+"T23:59:59");
    query.exec();
    while(query.next())
    {
        sets = new QBarSet(query.value(1).toString());//+query.value(2).toString().mid(0,9)+query.value(2).toString().mid(11)
        *sets << query.value(3).toInt();
        series->append(sets);
        //categories << query.value(1).toString()+query.value(2).toString().mid(0,9)+query.value(2).toString().mid(11);
        ui->textBrowser->append(QString::number(cnt+1)+"、于"+query.value(2).toString().mid(0,9)+"日"+query.value(2).toString().mid(11)+"和"+
                                query.value(1).toString()+"通信"+query.value(3).toString()+"分钟");
        cnt++;
    }

    query.prepare("select * from tb_tel where otherside = :other_temp and starttime > :times and starttime < :timee;");
    query.bindValue(":other_temp",ui->lineEdit1->text());
    query.bindValue(":times",times);
    query.bindValue(":timee",timee+"T23:59:59");
    query.exec();
    while(query.next())
    {
        sets = new QBarSet(query.value(0).toString());//+query.value(2).toString().mid(0,9)+query.value(2).toString().mid(11)
        *sets << query.value(3).toInt();
        series->append(sets);
        //categories << query.value(0).toString()+query.value(2).toString().mid(0,9)+query.value(2).toString().mid(11);
        ui->textBrowser->append(QString::number(cnt+1)+"、于"+query.value(2).toString().mid(0,9)+"日"+query.value(2).toString().mid(11)+"和"+
                                query.value(0).toString()+"通讯"+query.value(3).toString()+"分钟");
        cnt++;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(times+"至"+timee+"间的通话时长分析");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    //坐标轴//
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    //axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,200);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    axisY->setGridLineVisible(true);          //网格显示
    axisY->setMinorTickCount(4);              //小刻度
    axisY->setTitleText("通讯时间(分钟)");

    //柱状中lable位置,显示数据的值
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    series->setLabelsVisible(true);
    series->setLabelsAngle(90);

    //显示图例//
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // 创建QChartView 对象
    QChartView *chartView;
    // QChartView 对象载入饼状图
    chartView = new QChartView(chart);
    // 显示图标
    ui->verticalLayout->insertWidget(0,chartView);
    helper->destroyConn();
}
