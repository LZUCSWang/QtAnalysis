#ifndef QUERY_H
#define QUERY_H

#include <QMainWindow>
#include "query_fir.h"
#include "query_sec.h"
#include "query_thi.h"
namespace Ui {
class Query;
}

class Query : public QMainWindow
{
    Q_OBJECT

public:
    explicit Query(QWidget *parent = 0);
    ~Query();
    Query_fir *query_fir = NULL;//用来保存查询页面的实例化对象的地址
    Query_sec *query_sec = NULL;
    Query_thi *query_thi = NULL;
signals:
    void back_query();
private slots:
    //void on_pbt_clicked();//之前有次BUG就是因为这条错误语句，没有名为pbt的按钮，注意注意！
    void on_pbt1_clicked();
    void on_pbt2_clicked();

    void on_pbt3_clicked();

private:
    Ui::Query *ui;
};

#endif // QUERY_H
