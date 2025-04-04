#ifndef QUERY_THI_H
#define QUERY_THI_H

#include <QMainWindow>

namespace Ui {
class Query_thi;
}

class Query_thi : public QMainWindow
{
    Q_OBJECT

public:
    explicit Query_thi(QWidget *parent = 0);
    ~Query_thi();
    void tbshow(QString timestart, QString timeend);
signals:
    void back_query_thi();
private:
    Ui::Query_thi *ui;
};

#endif // QUERY_THI_H
