#ifndef MONEY_H
#define MONEY_H

#include <QDialog>

namespace Ui {
class money;
}

class money : public QDialog
{
    Q_OBJECT
signals:
    void sendMoney(int,int,int);
public:
    explicit money(QWidget *parent = nullptr);
    ~money();

private slots:
    void on_money_apply_clicked();

    void on_range_apply_clicked();

private:
    Ui::money *ui;
    int m_flag,m_sMoney,m_eMoney;
};

#endif // MONEY_H
