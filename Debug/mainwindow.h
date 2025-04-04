#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "telwidget.h"
#include "paywidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_telWidget_clicked();

    void on_payWidget_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
