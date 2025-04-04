#ifndef USER_S_H
#define USER_S_H

#include <QMainWindow>

namespace Ui {
class User_s;
}

class User_s : public QMainWindow
{
    Q_OBJECT

public:
    explicit User_s(QWidget *parent = 0);
    ~User_s();
signals:
    void back_user_s();
private slots:
    void on_pbt1_clicked();

private:
    Ui::User_s *ui;
};

#endif // USER_S_H
