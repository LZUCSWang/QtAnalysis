#ifndef PAYWIDGET_H
#define PAYWIDGET_H

#include <QWidget>
#include "excelengine.h"
#include "excelnode.h"
#include "themewidget.h"
#include "date.h"
#include "money.h"
#include "graphwidget.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QColor>
#include <QErrorMessage>
#include <QTableWidgetItem>
#include <QListWidgetItem>
#include <QMouseEvent>
namespace Ui {
class payWidget;
}

class payWidget : public QWidget
{
    Q_OBJECT

public:
    explicit payWidget(QWidget *parent = nullptr);
    ~payWidget();
    QVariantList dataList;
    QVariantList *nextDataList;
    ExcelEngine *excel;
    bool colorFlag[99999];
    QVariantList* historyDataList[999];
    int historyDataListIndex;
    int chartMethod;
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void sendColor(QColor);
    void sendData(QVariantList&);
    void sendDataTable(DataTable&,QList<QString>&);
    //    void setCenter();
private slots:
    void on_analyse_clicked();

    void on_date_clicked();

    void on_money_clicked();

    void on_addfile_clicked();

    void on_importfile_clicked();

    void updateMain(QVariantList &);

    void on_apply_clicked();

    void receiveDate(int, QDate ,QDate );

    void receiveMoney(int,int,int);

    void on_tableWidget_cellClicked(int row, int column);
    void on_onlyColored_stateChanged(int arg1);

    void on_enableDateFilter_stateChanged(int arg1);

    void on_enableMoneyFilter_stateChanged(int arg1);

    void on_pushButton_3_clicked();

    void on_historyDataList_activated(int index);

    void on_history_textChanged();

    void on_analyseResult_textChanged();

    void on_showChart_stateChanged(int arg1);

    void on_tempSave_clicked();

    void on_outputFile_clicked();

    void on_Output_listWidget_itemClicked(QListWidgetItem *item);

    void on_clear_output_clicked();

    void on_pushButton_clicked();

    void on_grab_clicked();

    void on_refresh_clicked();

    void on_OutputImages_listWidget_itemClicked(QListWidgetItem *item);

    void on_enableDateSort_clicked();

    void on_enableMoneySort_clicked();

    void on_enableDateSort_down_clicked();

    void on_enableMoneySort_down_clicked();

    void on_outputDelete_pushButton_clicked();

    void on_sigle_outputFile_clicked();

    void on_message_stateChanged(int arg1);

    void on_importfile_5_clicked();

private:
    QDate sDate,eDate;
    int money_flag,sMoney,eMoney,date_flag;
    ThemeWidget *widget;
    date *m_date;
    money *m_money;
    int enableDateFilter;
    int enableMoneyFilter;
    int enableColorFilter;
    bool enableDateSort;
    bool enableMoneySort;
    bool enableDateSort_down;
    bool enableMoneySort_down;
    bool dateToFilter[999];
    bool moneyToFilter[999];
    int flagOut[999];
    GraphWidget *graphicsView;
    int charMethod(QVariantList&);
    void analyse(QVariantList&datalist);
    DataTable m_datatable;
    QList<QString>pieName;
    int showChart;
    int haveSaved;
    bool notToSave[999];
    QListWidgetItem *seletedOutputQListWidgetItem;
    Ui::payWidget *ui;
    bool OutputImages_listWidget_itemClicked_Flag;
};

#endif // PAYWIDGET_H
