#include "money.h"
#include "ui_money.h"

money::money(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::money)
{
    ui->setupUi(this);
    int nMin = 0;
    int nMax = 2000;
    int nSingleStep = 10;
    ui->money_spinBox->setMinimum(nMin);  // 最小值
    ui->money_spinBox->setMaximum(nMax);  // 最大值
    ui->money_spinBox->setSingleStep(nSingleStep);  // 步长

    ui->money_horizontalSlider->setMinimum(nMin);  // 最小值
    ui->money_horizontalSlider->setMaximum(nMax);  // 最大值
    ui->money_horizontalSlider->setSingleStep(nSingleStep);  // 步长

    connect(ui->money_spinBox, SIGNAL(valueChanged(int)), ui->money_horizontalSlider, SLOT(setValue(int)));
    connect(ui->money_horizontalSlider, SIGNAL(valueChanged(int)), ui->money_spinBox, SLOT(setValue(int)));

    ui->s_spinBox->setMinimum(nMin);  // 最小值
    ui->s_spinBox->setMaximum(nMax);  // 最大值
    ui->s_spinBox->setSingleStep(nSingleStep);  // 步长

    ui->s_horizontalSlider->setMinimum(nMin);  // 最小值
    ui->s_horizontalSlider->setMaximum(nMax);  // 最大值
    ui->s_horizontalSlider->setSingleStep(nSingleStep);  // 步长

    connect(ui->s_spinBox, SIGNAL(valueChanged(int)), ui->s_horizontalSlider, SLOT(setValue(int)));
    connect(ui->s_horizontalSlider, SIGNAL(valueChanged(int)), ui->s_spinBox, SLOT(setValue(int)));

    ui->e_spinBox->setMinimum(nMin);  // 最小值
    ui->e_spinBox->setMaximum(nMax);  // 最大值
    ui->e_spinBox->setSingleStep(nSingleStep);  // 步长

    ui->e_horizontalSlider->setMinimum(nMin);  // 最小值
    ui->e_horizontalSlider->setMaximum(nMax);  // 最大值
    ui->e_horizontalSlider->setSingleStep(nSingleStep);  // 步长

    connect(ui->e_spinBox, SIGNAL(valueChanged(int)), ui->e_horizontalSlider, SLOT(setValue(int)));
    connect(ui->e_horizontalSlider, SIGNAL(valueChanged(int)), ui->e_spinBox, SLOT(setValue(int)));


}

money::~money()
{
    delete ui;
}

void money::on_money_apply_clicked()
{
    if (ui->low_radioButton->isChecked())m_flag=1;
    else if (ui->eLow_radioButton->isChecked())m_flag=2;
    else if (ui->equal_radioButton->isChecked())m_flag=3;
    else if (ui->eLarge_radioButton->isChecked())m_flag=4;
    else if (ui->large_radioButton->isChecked())m_flag=5;
    else {}//message box
    m_sMoney=m_eMoney=ui->money_spinBox->value();
    emit sendMoney(m_flag,m_sMoney,m_eMoney);
}

void money::on_range_apply_clicked()
{
    m_flag=6;
    m_sMoney=ui->s_spinBox->value();
    m_eMoney=ui->e_spinBox->value();
    emit sendMoney(m_flag,m_sMoney,m_eMoney);
}
