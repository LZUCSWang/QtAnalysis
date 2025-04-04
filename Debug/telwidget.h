#ifndef TELWIDGET_H
#define TELWIDGET_H

#include <QWidget>
#include "query.h"
#include "analy.h"
namespace Ui {
class telWidget;
}

class telWidget : public QWidget
{
    Q_OBJECT

public:
    explicit telWidget(QWidget *parent = nullptr);
    ~telWidget();
    Query *query = NULL;//用来保存查询页面的实例化对象的地址
    Analy *analy = NULL;//用来保存分析页面的实例化对象的地址

private slots:
    void on_pbt3_clicked();

private:
    Ui::telWidget *ui;
};

#endif // TELWIDGET_H
