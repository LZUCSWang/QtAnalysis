#include "date.h"
#include "ui_date.h"
#include <QDebug>
date::date(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::date)
{
    ui->setupUi(this);
}
date::~date()
{
    delete ui;
}

void date::on_calendarWidget_clicked(const QDate &date)
{
    m_sDate=m_eDate=date;
    ui->dateEdit->setDate(date);
}
void date::on_dateEdit_userDateChanged(const QDate &date)
{
    m_sDate=m_eDate=date;
}
void date::on_s_dateEdit_userDateChanged(const QDate &date)
{
    m_sDate=date;
}
void date::on_e_dateEdit_userDateChanged(const QDate &date)
{
    m_eDate=date;
}
void date::on_s_calendarWidget_clicked(const QDate &date)
{
    m_sDate=date;
    ui->s_dateEdit->setDate(date);
}
void date::on_e_calendarWidget_clicked(const QDate &date)
{
    m_eDate=date;
    ui->e_dateEdit->setDate(date);
}

void date::on_applyDates_clicked()
{

    emit sendDate(6,m_sDate,m_eDate);
}

void date::on_applyDate_clicked()
{
    if (ui->low_radioButton->isChecked())flag=1;
    else if (ui->elow_radioButton->isChecked())flag=2;
    else if (ui->e_radioButton->isChecked())flag=3;
    else if (ui->elarge_radioButton->isChecked())flag=4;
    else if (ui->large_radioButton->isChecked())flag=5;
    else {}//message box
    emit sendDate(flag,m_sDate,m_eDate);
}
