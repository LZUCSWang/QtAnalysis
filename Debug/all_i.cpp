#include "all_i.h"
#include "ui_all_i.h"
#include "DBHelper.h"
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QtCharts>
All_i::All_i(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::All_i)
{
    ui->setupUi(this);
    connect(ui->pbt2,&QPushButton::clicked,[=](){
        //发一个信号过去
        emit this->back_all();
    });
}

All_i::~All_i()
{
    delete ui;
}

void All_i::on_pbt1_clicked()
{
    DBHelper *helper=DBHelper::getInstance();
    helper->creatConn();
    QSqlQuery query;
    int cnt = 0;
    int more = 0;
    int less = 0;
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

    QString times = ui->lineEdit1->text() + '-' +
            (ui->lineEdit2->text().toInt()<10 ? '0'+ui->lineEdit2->text():ui->lineEdit2->text()) + '-' +
            (ui->lineEdit3->text().toInt()<10 ? '0'+ui->lineEdit3->text():ui->lineEdit3->text());
    QString timee = ui->lineEdit4->text() + '-' +
            (ui->lineEdit5->text().toInt()<10 ? '0'+ui->lineEdit5->text():ui->lineEdit5->text()) + '-' +
            (ui->lineEdit6->text().toInt()<10 ? '0'+ui->lineEdit6->text():ui->lineEdit6->text());

    query.prepare("select * from tb_tel where starttime > :times and starttime < :timee order by duration desc;");
    query.bindValue(":times",times);
    query.bindValue(":timee",timee+"T23:59:59");
    query.exec();
    ui->textBrowser->append("通讯时长不少于"+ui->lineEdit7->text()+"分钟的记录有:");
    while(query.next())
    {
        if(query.value(3).toInt()>=ui->lineEdit7->text().toInt())
        {
            more++;
            ui->textBrowser->append(QString::number(cnt+1)+"、"+query.value(0).toString()+"于"+query.value(2).toString().mid(0,9)+"日"+
                                    query.value(2).toString().mid(11)+"和"+query.value(1).toString()+"通信"+query.value(3).toString()+"分钟");
            cnt++;
        }
        else
            less++;
    }

    series->append("不少于", more);
    series->append("少于", less);
    series->setHoleSize(0);//饼图中间是否需要填洞
    series->setPieSize(1);//设置大小：这里的大小就是占据整个view的大小
    //series->setLabelsVisible();
    QChart* chart = new QChart();
    chart->addSeries(series);// 将创建好的饼图对象添加进QChart
    chart->setTitle("联络时间不少于/少于"+ui->lineEdit7->text()+"分钟占比");
    chart->setAnimationOptions(QChart::AllAnimations);//设置动画效果
    chart->legend()->setAlignment(Qt::AlignRight);

    // 获取饼图中某一个切片（在绘制圆环图Demo中是通过appent函数获取，这里换一种方式）
    for(int i=0;i<series->count();i++)//处理每个切片对应的维度
    {
        slice = series->slices().at(i);
        slice->setLabel(QString::asprintf("%s:%.0f,%.1f%%",slice->label().toStdString().data(),slice->value(),slice->percentage()*100));//不能放在前面，延时
    }

    // 创建QChartView 对象
    QChartView *chartView;
    // QChartView 对象载入饼状图
    chartView = new QChartView(chart);
    // 显示图标
    ui->verticalLayout->insertWidget(0,chartView);
    helper->destroyConn();
}
