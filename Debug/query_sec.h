#ifndef QUERY_SEC_H
#define QUERY_SEC_H

#include <QMainWindow>

namespace Ui {
class Query_sec;
}

class Query_sec : public QMainWindow
{
    Q_OBJECT

public:
    explicit Query_sec(QWidget *parent = 0);
    ~Query_sec();
    void tbshow(QString destine);
signals:
    void back_query_sec();
private:
    Ui::Query_sec *ui;
};

#endif // QUERY_SEC_H
