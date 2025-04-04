#include "analy.h"
#include "ui_analy.h"

Analy::Analy(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Analy)
{
    ui->setupUi(this);
    connect(ui->pbt4,&QPushButton::clicked,[=](){
        //发一个信号过去
        emit this->back_analy();
    });
    this->user_s = new User_s;//实例化分析页面下用户特定时间分析页面
    connect(ui->pbt1,&QPushButton::clicked,[=](){
        //所谓的切换窗口，其实就是把主窗口隐藏
        //第四个页面显示
        this->hide();//隐藏主菜单
        this->user_s->show();//显示第四个窗口
    });
    connect(this->user_s,&User_s::back_user_s,[=](){
        this->user_s->hide();
        this->show();
    });



    this->user_i = new User_i;//实例化分析页面下用户时间区间分析页面
    connect(ui->pbt2,&QPushButton::clicked,[=](){
        //所谓的切换窗口，其实就是把主窗口隐藏
        //第五个页面显示
        this->hide();//隐藏主菜单
        this->user_i->show();//显示第五个窗口
    });
    connect(this->user_i,&User_i::back_user_i,[=](){
        this->user_i->hide();
        this->show();
    });



    this->all_i = new All_i;//实例化分析页面下整体时间区间分析页面
    connect(ui->pbt3,&QPushButton::clicked,[=](){
        //所谓的切换窗口，其实就是把主窗口隐藏
        //第六个页面显示
        this->hide();//隐藏主菜单
        this->all_i->show();//显示第六个窗口
    });
    connect(this->all_i,&All_i::back_all,[=](){
        this->all_i->hide();
        this->show();
    });
}

Analy::~Analy()
{
    delete ui;
}
