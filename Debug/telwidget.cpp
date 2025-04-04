#include "telwidget.h"
#include "ui_telwidget.h"
#include "query.h"
#include "analy.h"
telWidget::telWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::telWidget)
{
    ui->setupUi(this);
    this->query = new Query;//实例化查询页面
    connect(ui->pbt1,&QPushButton::clicked,[=](){
        //所谓的切换窗口，其实就是把主窗口隐藏
        //第二个页面显示
        this->hide();
        this->query->show();//显示第二个窗口
    });
    connect(this->query,&Query::back_query,[=](){
        this->query->hide();
        this->show();
    });

    this->analy = new Analy;//实例化分析页面
    connect(ui->pbt2,&QPushButton::clicked,[=](){
        //所谓的切换窗口，其实就是把主窗口隐藏
        //第三个页面显示
        this->hide();//隐藏主菜单
        this->analy->show();//显示第三个窗口
    });
    connect(this->analy,&Analy::back_analy,[=](){
        this->analy->hide();
        this->show();
    });
}

telWidget::~telWidget()
{
    delete ui;
}

void telWidget::on_pbt3_clicked()
{
    this->close();
}
