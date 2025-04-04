#ifndef USER_I_H
#define USER_I_H

#include <QMainWindow>

namespace Ui {
class User_i;
}

class User_i : public QMainWindow
{
    Q_OBJECT

public:
    explicit User_i(QWidget *parent = 0);
    ~User_i();
signals:
    void back_user_i();
private slots:
    void on_pbt1_clicked();

private:
    Ui::User_i *ui;
};

#endif // USER_I_H
