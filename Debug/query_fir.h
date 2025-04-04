#ifndef QUERY_FIR_H
#define QUERY_FIR_H

#include <QMainWindow>

namespace Ui {
class Query_fir;
}

class Query_fir : public QMainWindow
{
    Q_OBJECT

public:
    explicit Query_fir(QWidget *parent = 0);
    ~Query_fir();
    void tbshow(QString us,QString other);
signals:
    void back_query_fir();
private:
    Ui::Query_fir *ui;
};

#endif // QUERY_FIR_H
