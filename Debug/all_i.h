#ifndef ALL_I_H
#define ALL_I_H

#include <QMainWindow>

namespace Ui {
class All_i;
}

class All_i : public QMainWindow
{
    Q_OBJECT

public:
    explicit All_i(QWidget *parent = 0);
    ~All_i();
signals:
    void back_all();
private slots:
    void on_pbt1_clicked();

private:
    Ui::All_i *ui;
};

#endif // ALL_I_H
