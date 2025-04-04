#ifndef DATE_H
#define DATE_H

#include <QDialog>
#include <QDate>
namespace Ui {
class date;
}

class date : public QDialog
{
    Q_OBJECT

public:
    explicit date(QWidget *parent = nullptr);
    ~date();
signals:
    void sendDate(int,QDate,QDate);
private slots:
    void on_calendarWidget_clicked(const QDate &date);

    void on_s_dateEdit_userDateChanged(const QDate &date);

    void on_dateEdit_userDateChanged(const QDate &date);

    void on_e_dateEdit_userDateChanged(const QDate &date);

    void on_s_calendarWidget_clicked(const QDate &date);

    void on_e_calendarWidget_clicked(const QDate &date);

    void on_applyDates_clicked();

    void on_applyDate_clicked();

private:
    Ui::date *ui;
    QDate m_sDate,m_eDate;
    int flag;
};

#endif // DATE_H
