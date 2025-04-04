#ifndef ANALY_H
#define ANALY_H

#include <QMainWindow>
#include "user_s.h"
#include "user_i.h"
#include "all_i.h"
namespace Ui {
class Analy;
}

class Analy : public QMainWindow
{
    Q_OBJECT

public:
    explicit Analy(QWidget *parent = 0);
    ~Analy();
    User_s *user_s = NULL;//用来保存分析页面下用户特定时间分析页面的实例化对象的地址
    User_i *user_i = NULL;//用来保存分析页面下用户时间区间分析页面的实例化对象的地址
    All_i *all_i = NULL;//用来保存分析页面下整体时间区间分析页面的实例化对象的地址
signals:
    void back_analy();
private:
    Ui::Analy *ui;
};

#endif // ANALY_H
