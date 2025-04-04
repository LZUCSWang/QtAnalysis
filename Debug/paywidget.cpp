#include "paywidget.h"
#include "ui_paywidget.h"
#include <QHeaderView>
QString showName="请先拖动某结点";
int messageFlag=0;
//QList<QPair<QImage,QString>> images;
QMap<QString,QPixmap>images;
payWidget::payWidget(QWidget *parent) :
    showChart(0),
    historyDataListIndex(0),
    QWidget(parent),
    excel(new ExcelEngine),
    m_date(new date),
    m_money(new money),
    ui(new Ui::payWidget)
{
    enableMoneySort=false;
    enableDateSort=false;
    enableDateSort_down=false;
    enableMoneySort_down=false;
    ui->setupUi(this);
    ui->imageShow->installEventFilter(this);
    ui->history->insertPlainText("初始化\n");
    connect(m_date,SIGNAL(sendDate(int,QDate,QDate)),this,SLOT(receiveDate(int,QDate,QDate)));
    connect(m_money,SIGNAL(sendMoney(int,int,int)),this,SLOT(receiveMoney(int,int,int)));
    connect(this,SIGNAL(sendData(QVariantList&)),ui->graphicsView,SLOT(receiveData(QVariantList&)));
    //    on_importfile_clicked();
    haveSaved=0;
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memset(flagOut,0,sizeof (flagOut));
}
payWidget::~payWidget()
{
    delete ui;
}
void payWidget::on_analyse_clicked()
{
    ui->history->insertPlainText("点击“分析”按钮\n");
    if(dataList.empty())return;
    m_datatable.clear();
    analyse(dataList);
    if(showChart==2){
        widget = new ThemeWidget();
        connect(this,SIGNAL(sendDataTable(DataTable&,QList<QString>&)),widget,SLOT(receiveDataTable(DataTable&,QList<QString>&)));
        emit sendDataTable(m_datatable,pieName);
        widget->showFullScreen();
    }
}
void payWidget::on_date_clicked()
{
    m_date->show();
}
void payWidget::on_money_clicked()
{
    m_money->show();
}
void payWidget::on_addfile_clicked()
{
    QString sPath = QFileDialog::getOpenFileName(this,"选择要导入的xls文件",".","c++ files(*.xls);;");
    if(sPath!="")
    {
        ui->Input_listWidget->addItem(sPath);
        ui->importfile->setEnabled(true);
    }
}
void payWidget::on_importfile_clicked()
{
    ui->importfile->setEnabled(false);
    ui->addfile->setEnabled(false);
    int i = 0, nCnt = ui->Input_listWidget->count();
    for (i = 0; i < nCnt; ++i)
    {
        QListWidgetItem* pItem = ui->Input_listWidget->item(i);
        if (pItem)
        {
            historyDataList[historyDataListIndex]=new QVariantList();
            excel->ReadExcel(pItem->text(),*historyDataList[historyDataListIndex]);
            pItem->setIcon(QIcon(":/icons/8666665_check.png"));
            dataList=*historyDataList[historyDataListIndex++];
        }
    }
    emit sendData(dataList);
    updateMain(dataList);
    ui->analyse->setEnabled(true);
    ui->showChart->setEnabled(true);
    ui->tempSave->setEnabled(true);
    ui->date->setEnabled(true);
    ui->money->setEnabled(true);
    ui->apply->setEnabled(true);
    ui->historyDataList->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
}
void payWidget::updateMain(QVariantList&datalist){
    if (datalist.empty()){
        QErrorMessage* errordlg = new QErrorMessage(this);
        errordlg->setWindowTitle("错误对话框");
        errordlg->showMessage("你还没有导入文件！");
        return;
    }
    chartMethod=charMethod(dataList);
    if(chartMethod==4){
        ui->showChart->setCheckState(Qt::Unchecked);
        ui->showChart->setEnabled(false);
    }
    else{
        ui->showChart->setEnabled(true);
    }
    int tableWidget_row,tableWidget_col;
    tableWidget_row=datalist.size();
    tableWidget_col=datalist[0].toList().size();
    ui->tableWidget->setRowCount(tableWidget_row);
    ui->tableWidget->setColumnCount(tableWidget_col);
    for (int i=0;i<tableWidget_row;i++){
        for (int j=0;j<tableWidget_col;j++)
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(datalist[i].toList()[j].toString()));
    }
    ui->history->insertPlainText("更新表格:"+ui->historyDataList->currentText()+"\n");
}
void payWidget::on_apply_clicked()
{
    QString message="点击“应用”按钮,筛选条件为:\n";
    if(enableDateFilter==2)
    {
        message+=ui->dateFilter->text()+"\n";
        memset(dateToFilter,0,sizeof(dateToFilter));
        for (int i=0;i<dataList.size();i++){
            QDate date=QDate::fromString(dataList[i].toList()[3].toString().mid(0,10),"yyyy-MM-dd");
            switch (date_flag) {
            case 1:if(date>=sDate)dateToFilter[i]=true;break;
            case 2:if(date>sDate)dateToFilter[i]=true;break;
            case 3:if(date!=sDate)dateToFilter[i]=true;break;
            case 4:if(date<sDate)dateToFilter[i]=true;break;
            case 5:if(date<=sDate)dateToFilter[i]=true;break;
            case 6:if(date>eDate||date<sDate)dateToFilter[i]=true;break;
            default:break;
            }
        }
    }
    if(enableMoneyFilter==2)
    {
        message+=ui->moneyFilter->text()+"\n";
        memset(moneyToFilter,0,sizeof(moneyToFilter));
        for(int i=0;i<dataList.size();i++){
            int money=dataList[i].toList()[2].toInt();
            switch (money_flag) {
            case 1:if(money>=sMoney)moneyToFilter[i]=true;break;
            case 2:if(money>sMoney)moneyToFilter[i]=true;break;
            case 3:if(money!=sMoney)moneyToFilter[i]=true;break;
            case 4:if(money<sMoney)moneyToFilter[i]=true;break;
            case 5:if(money<=sMoney)moneyToFilter[i]=true;break;
            case 6:if(money<sMoney||money>eMoney)moneyToFilter[i]=true;break;
            default:break;
            }
        }
    }
    if(enableColorFilter==2){
        message+="只显示上色的信息\n";
    }

    ui->history->insertPlainText(message);

    nextDataList=historyDataList[historyDataListIndex++]=new QVariantList();


    for (int i=0;i<dataList.size();i++){
        if ((enableDateFilter==2&&dateToFilter[i])||(enableMoneyFilter==2&&moneyToFilter[i])||(enableColorFilter==2&&!colorFlag[i]))continue;
        nextDataList->append(dataList[i]);
    }
    if(nextDataList->empty()){
        QMessageBox msgBox;
        msgBox.setText("筛选后的数据为空，请重设！");
        msgBox.exec();
        historyDataListIndex--;
        return;
    }
    if(enableDateSort)
    {qSort(nextDataList->begin(),nextDataList->end(),[](const QVariant &d1,const QVariant &d2)->bool{return QDateTime::fromString(d1.toList()[3].toString().replace("T"," ").mid(0,19),"yyyy-MM-dd hh:mm:ss")>QDateTime::fromString(d2.toList()[3].toString().replace("T"," ").mid(0,19),"yyyy-MM-dd hh:mm:ss");});ui->history->insertPlainText("日期降序\n");}
    else if(enableMoneySort)
    {qSort(nextDataList->begin(),nextDataList->end(),[](const QVariant &d1,const QVariant &d2)->bool{return d1.toList()[2].toInt()>d2.toList()[2].toInt();});ui->history->insertPlainText("金额降序\n");}
    else if(enableDateSort_down)
    {qSort(nextDataList->begin(),nextDataList->end(),[](const QVariant &d1,const QVariant &d2)->bool{return QDateTime::fromString(d1.toList()[3].toString().replace("T"," ").mid(0,19),"yyyy-MM-dd hh:mm:ss")<QDateTime::fromString(d2.toList()[3].toString().replace("T"," ").mid(0,19),"yyyy-MM-dd hh:mm:ss");});ui->history->insertPlainText("日期升序\n");}
    else if(enableMoneySort_down)
    {qSort(nextDataList->begin(),nextDataList->end(),[](const QVariant &d1,const QVariant &d2)->bool{return d1.toList()[2].toInt()<d2.toList()[2].toInt();});ui->history->insertPlainText("金额升序\n");}

    enableMoneySort=false;
    enableDateSort=false;
    enableDateSort_down=false;
    enableMoneySort_down=false;

    ui->historyDataList->addItem(QString::number(historyDataListIndex+1)+"."+ui->nameLineEdit->text());
    ui->nameLineEdit->clear();
    ui->historyDataList->setCurrentIndex(historyDataListIndex);

    dataList=*nextDataList;
    updateMain(dataList);

    ui->onlyColored->setCheckState(Qt::Unchecked);
    ui->onlyColored->setEnabled(false);
    ui->enableMoneyFilter->setCheckState(Qt::Unchecked);
    ui->enableMoneyFilter->setEnabled(false);
    ui->enableDateFilter->setCheckState(Qt::Unchecked);
    ui->enableDateFilter->setEnabled(false);

    ui->dateFilter->setText("日期:");
    ui->moneyFilter->setText("金额");
}
void payWidget::receiveDate(int m_flag,QDate m_sDate,QDate m_eDate){
    date_flag=m_flag;
    sDate=m_sDate;
    eDate=m_eDate;
    QString message1;
    message1="";
    if(sDate!=eDate){
        message1+=sDate.toString("yyyy-MM-dd")+" -- "+eDate.toString("yyyy-MM-dd");
    }
    else
    {
        QList<QString>temp;
        temp<<"小于"<<"小于等于"<<"等于"<<"大于等于"<<"大于";
        message1+=temp[date_flag-1]+sDate.toString("yyyy-MM-dd");
    }
    ui->dateFilter->setText(message1);
    ui->enableDateFilter->setEnabled(true);
}
void payWidget::receiveMoney(int m_flag,int s_money,int e_money){
    money_flag=m_flag;
    sMoney=s_money;
    eMoney=e_money;
    QString message1="";
    if(sMoney!=eMoney){
        message1+=QString::number(sMoney)+" -- "+QString::number(eMoney);
    }
    else
    {
        QList<QString>temp;
        temp<<"小于"<<"小于等于"<<"等于"<<"大于等于"<<"大于";
        message1+=temp[money_flag-1]+QString::number(sMoney);
    }
    ui->moneyFilter->setText(message1);
    ui->enableMoneyFilter->setEnabled(true);
}
void payWidget::on_tableWidget_cellClicked(int row, int column)
{
    ui->history->insertPlainText(QString("点击位于第%1行，第%2列的单元格，内容为%3\n").arg(row).arg(column).arg(ui->tableWidget->item(row,column)->text()));
    int tableWidget_row,tableWidget_col;
    tableWidget_row=ui->tableWidget->rowCount();
    tableWidget_col=ui->tableWidget->columnCount();
    memset(colorFlag,0,sizeof(colorFlag));
    for (int i=0;i<tableWidget_row;i++){
        for (int j=0;j<tableWidget_col;j++){
            ui->tableWidget->item(i,j)->setBackground(Qt::white);
        }
    }
    if (column==0){
        ui->onlyColored->setEnabled(true);
        QString to=ui->tableWidget->item(row,column)->text();
        for (int i=0;i<tableWidget_row;i++){
            if (ui->tableWidget->item(i,0)->text()==to){
                colorFlag[i]=true;
                ui->tableWidget->item(i, 0)->setBackground(Qt::red);
                ui->tableWidget->item(i,1)->setBackground(Qt::blue);
            }
        }
    }
    else if (column==1){
        ui->onlyColored->setEnabled(true);
        QString from=ui->tableWidget->item(row,column)->text();
        for (int i=0;i<tableWidget_row;i++){
            if (ui->tableWidget->item(i,1)->text()==from){
                colorFlag[i]=true;
                ui->tableWidget->item(i,0)->setBackground(Qt::red);
                ui->tableWidget->item(i,1)->setBackground(Qt::blue);
            }
        }
    }
    else{
        ui->onlyColored->setCheckState(Qt::Unchecked);
        ui->onlyColored->setEnabled(false);
    }
}
void payWidget::on_onlyColored_stateChanged(int arg1)
{
    enableColorFilter=arg1;
}
void payWidget::on_enableDateFilter_stateChanged(int arg1)
{
    enableDateFilter=arg1;
}
void payWidget::on_enableMoneyFilter_stateChanged(int arg1)
{
    enableMoneyFilter=arg1;
}
void payWidget::on_pushButton_3_clicked()
{
    ui->history->insertPlainText("选择点为:"+showName+"\n");
}
int payWidget::charMethod(QVariantList &datalist){
    if(dataList.empty())return 0;
    QString sender,receiver;
    receiver=datalist[0].toList()[0].toString();
    sender=datalist[0].toList()[1].toString();
    int f1=0,f2=0;
    for(int i=1;i<datalist.size();i++){
        QVariantList temp=datalist[i].toList();
        if(!f1&&temp[0].toString()!=receiver){
            f1=1;
        }
        if(!f2&&temp[1].toString()!=sender){
            f2=1;
        }
        if(f1&&f2){
            return 1;
        }
    }
    if(f1&&!f2){
        return 2;
    }
    if(!f1&&f2){
        return 3;
    }
    return 4;
}
void payWidget::analyse(QVariantList& datalist){
    if(chartMethod==1){//多人
        pieName.clear();
        pieName<<"发送次数"<<"接收次数"<<"发送金额"<<"接收金额"<<"每月交易金额量"<<"每月交易次数";
        int allMoney=0,allFrequency=0;
        QDate nDate;
        QMap<QString,int>fSender,fReceiver,mSender,mReceiver;
        QMap<QDate,int>monthMapMoney,monthMapFrequency;
        QMap<QString,int>::iterator iter;
        QMap<QDate,int>::iterator QDateIntiter;
        for (int i=0;i<datalist.size();i++){
            allFrequency++;
            QVariantList data=datalist[i].toList();
            int money=data[2].toInt();
            mSender[data[1].toString()]+=money;
            mReceiver[data[0].toString()]+=money;
            allMoney+=money;
            nDate=QDate::fromString(data[3].toString().mid(0,7),"yyyy-MM");
            monthMapMoney[nDate]+=money;
            monthMapFrequency[nDate]++;
            fSender[data[1].toString()]++;
            fReceiver[data[0].toString()]++;
        }
        DataList data;
        for(iter=fSender.begin();iter!=fSender.end();iter++){
            data<<Data(iter.value(),iter.key()+":"+QString::number(iter.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iter=fReceiver.begin();iter!=fReceiver.end();iter++){
            data<<Data(iter.value(),iter.key()+":"+QString::number(iter.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iter=mSender.begin();iter!=mSender.end();iter++){
            data<<Data(iter.value(),iter.key()+":"+QString::number(iter.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iter=mReceiver.begin();iter!=mReceiver.end();iter++){
            data<<Data(iter.value(),iter.key()+":"+QString::number(iter.value()));
        }
        m_datatable<<data;
        data.clear();
        for(QDateIntiter=monthMapMoney.begin();QDateIntiter!=monthMapMoney.end();QDateIntiter++){
            data<<Data(QDateIntiter.value(),QDateIntiter.key().toString("yyyy-MM")+":"+QString::number(QDateIntiter.value()));
        }
        m_datatable<<data;
        data.clear();
        for(QDateIntiter=monthMapFrequency.begin();QDateIntiter!=monthMapFrequency.end();QDateIntiter++){
            data<<Data(QDateIntiter.value(),QDateIntiter.key().toString("yyyy-MM")+":"+QString::number(QDateIntiter.value()));
        }
        m_datatable<<data;
        data.clear();

        for(int i=0;i<6;i++){
            qSort(m_datatable[i].begin(),m_datatable[i].end(),[](const Data &d1,const Data &d2)->bool{return d1.first>d2.first;});
        }
        ui->analyseResult->insertPlainText(QString("表格"+ui->historyDataList->currentText()+"分析结果:\n总交易金额:%1元\n总交易次数:%2\n发送次数最多:%3次\n接收次数最多:%4次\n发送金额最多:%5元\n接收金额最多:%6元\n交易金额最高的月份:%7\n交易次数最高的月份:%8\n").arg(allMoney).arg(allFrequency).arg(m_datatable[0][0].second).arg(m_datatable[1][0].second).arg(m_datatable[2][0].second).arg(m_datatable[3][0].second).arg(m_datatable[4][0].second).arg(m_datatable[5][0].second));
    }
    else if(chartMethod==2){//单人(发送)
        pieName.clear();
        QString Sender=datalist[0].toList()[1].toString();
        pieName<<Sender+"发送金额"<<Sender+"发送频率"<<Sender+"每月发送金额"<<Sender+"每月发送频率"<<Sender+"每时发送金额"<<Sender+"每时发送频率";
        QMap<QString,int> ReceiverFrequency,ReceiverMoney;
        QMap<QDate,int>MonthMoney,MonthFrequency;
        QMap<int,int>HourMoney,HourFrequency;
        int allMoney=0,allFrequency=0;
        for(int i=0;i<datalist.size();i++){
            QVariantList data=datalist[i].toList();
            int money=data[2].toInt();
            allMoney+=money;
            allFrequency++;
            QString timeString=data[3].toString();
            QDate nDate=QDate::fromString(timeString.mid(0,7),"yyyy-MM");
            QTime nTime=QTime::fromString(timeString.mid(11,8),"hh:mm:ss");
            QString Receiver;
            MonthMoney[nDate]+=money;
            MonthFrequency[nDate]++;
            HourMoney[nTime.hour()]+=money;
            HourFrequency[nTime.hour()]++;
            Receiver=data[0].toString();
            ReceiverMoney[Receiver]+=money;
            ReceiverFrequency[Receiver]++;
        }
        QMap<QString,int>::iterator iter;
        QMap<QDate,int>::iterator iterDate;
        QMap<int,int>::iterator iterDateTime;
        DataList data;
        for(iter=ReceiverMoney.begin();iter!=ReceiverMoney.end();iter++){
            data<<Data(iter.value(),iter.key()+":"+QString::number(iter.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iter=ReceiverFrequency.begin();iter!=ReceiverFrequency.end();iter++){
            data<<Data(iter.value(),iter.key()+":"+QString::number(iter.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iterDate=MonthMoney.begin();iterDate!=MonthMoney.end();iterDate++){
            data<<Data(iterDate.value(),iterDate.key().toString("yyyy-MM")+":"+QString::number(iterDate.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iterDate=MonthFrequency.begin();iterDate!=MonthFrequency.end();iterDate++){
            data<<Data(iterDate.value(),iterDate.key().toString("yyyy-MM")+":"+QString::number(iterDate.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iterDateTime=HourMoney.begin();iterDateTime!=HourMoney.end();iterDateTime++){
            data<<Data(iterDateTime.value(),QString::number(iterDateTime.key())+":"+QString::number(iterDateTime.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iterDateTime=HourFrequency.begin();iterDateTime!=HourFrequency.end();iterDateTime++){
            data<<Data(iterDateTime.value(),QString::number(iterDateTime.key())+":"+QString::number(iterDateTime.value()));
        }
        m_datatable<<data;
        data.clear();

        for(int i=0;i<6;i++){
            qSort(m_datatable[i].begin(),m_datatable[i].end(),[](const Data &d1,const Data &d2)->bool{return d1.first>d2.first;});
        }
        ui->analyseResult->insertPlainText(QString("表格"+ui->historyDataList->currentText()+"分析结果:\n总交易金额:%1\n总交易次数:%2\n"+Sender+"发送金额最多的人:%3元\n"+Sender+"发送次数最多的人:%4次\n"+Sender+"发送金额最多的月:%5元\n"+Sender+"发送次数最多的月:%6次\n"+Sender+"发送金额最多的时间段:%7时\n"+Sender+"发送次数最多的时间段:%8时\n").arg(allMoney).arg(allFrequency).arg(m_datatable[0][0].second).arg(m_datatable[1][0].second).arg(m_datatable[2][0].second).arg(m_datatable[3][0].second).arg(m_datatable[4][0].second).arg(m_datatable[5][0].second));

    }
    else if(chartMethod==3){//单人(接收)
        pieName.clear();
        QString Receiver=datalist[0].toList()[0].toString();
        pieName<<Receiver+"接受金额"<<Receiver+"接受频率"<<Receiver+"每月接受金额"<<Receiver+"每月接受频率"<<Receiver+"每时接受金额"<<Receiver+"每时接收金额";
        QMap<QString,int> SenderFrequency,SenderMoney;
        QMap<QDate,int>MonthMoney,MonthFrequency;
        QMap<int,int>HourMoney,HourFrequency;
        int allMoney=0,allFrequency=0;
        for(int i=0;i<datalist.size();i++){
            QVariantList data=datalist[i].toList();
            int money=data[2].toInt();
            allMoney+=money;
            allFrequency++;
            QString timeString=data[3].toString();
            QDate nDate=QDate::fromString(timeString.mid(0,7),"yyyy-MM");
            QTime nTime=QTime::fromString(timeString.mid(11,8),"hh:mm:ss");
            QString Sender;
            MonthMoney[nDate]+=money;
            MonthFrequency[nDate]++;
            HourMoney[nTime.hour()]+=money;
            HourFrequency[nTime.hour()]++;
            Sender=data[1].toString();
            SenderMoney[Sender]+=money;
            SenderFrequency[Sender]++;
        }
        QMap<QString,int>::iterator iter;
        QMap<QDate,int>::iterator iterDate;
        QMap<int,int>::iterator iterDateTime;
        DataList data;
        for(iter=SenderMoney.begin();iter!=SenderMoney.end();iter++){
            data<<Data(iter.value(),iter.key()+":"+QString::number(iter.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iter=SenderFrequency.begin();iter!=SenderFrequency.end();iter++){
            data<<Data(iter.value(),iter.key()+":"+QString::number(iter.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iterDate=MonthMoney.begin();iterDate!=MonthMoney.end();iterDate++){
            data<<Data(iterDate.value(),iterDate.key().toString("yyyy-MM")+":"+QString::number(iterDate.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iterDate=MonthFrequency.begin();iterDate!=MonthFrequency.end();iterDate++){
            data<<Data(iterDate.value(),iterDate.key().toString("yyyy-MM")+":"+QString::number(iterDate.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iterDateTime=HourMoney.begin();iterDateTime!=HourMoney.end();iterDateTime++){
            data<<Data(iterDateTime.value(),QString::number(iterDateTime.key())+":"+QString::number(iterDateTime.value()));
        }
        m_datatable<<data;
        data.clear();
        for(iterDateTime=HourFrequency.begin();iterDateTime!=HourFrequency.end();iterDateTime++){
            data<<Data(iterDateTime.value(),QString::number(iterDateTime.key())+":"+QString::number(iterDateTime.value()));
        }
        m_datatable<<data;
        data.clear();

        for(int i=0;i<6;i++){
            qSort(m_datatable[i].begin(),m_datatable[i].end(),[](const Data &d1,const Data &d2)->bool{return d1.first>d2.first;});
        }
        ui->analyseResult->insertPlainText(QString("表格"+ui->historyDataList->currentText()+"分析结果:\n总交易金额:%1\n总交易次数:%2\n"+Receiver+"接收金额最多的人:%3元\n"+Receiver+"接收次数最多的人:%4次\n"+Receiver+"接收金额最多的月:%5元\n"+Receiver+"接收次数最多的月:%6次\n"+Receiver+"接收金额最多的时间段:%7元\n"+Receiver+"接收次数最多的时间段:%18次\n").arg(allMoney).arg(allFrequency).arg(m_datatable[0][0].second).arg(m_datatable[1][0].second).arg(m_datatable[2][0].second).arg(m_datatable[3][0].second).arg(m_datatable[4][0].second).arg(m_datatable[5][0].second));
    }
    else if(chartMethod==4){
        QString Receiver,Sender;
        Receiver=datalist[0].toList()[0].toString();
        Sender=datalist[0].toList()[1].toString();
        int allMoney=0;
        int frequency=0;
        for(int i=0;i<datalist.size();i++){
            QVariantList data=datalist[i].toList();
            int money=data[2].toInt();
            allMoney+=money;
            frequency++;
        }
        ui->analyseResult->insertPlainText(QString("表格"+ui->historyDataList->currentText()+"分析结果:\n发送方:%1       接收方:%2\n总交易金额:%3\n总交易次数:%4\n").arg(Sender).arg(Receiver).arg(allMoney).arg(frequency));
    }
}
void payWidget::on_historyDataList_activated(int index)
{
    dataList=*historyDataList[index];
    updateMain(dataList);
}
void payWidget::on_history_textChanged()
{
    ui->history->moveCursor(QTextCursor::End);
}
void payWidget::on_analyseResult_textChanged()
{
    ui->analyseResult->moveCursor(QTextCursor::End);
}
void payWidget::on_showChart_stateChanged(int arg1)
{
    showChart=arg1;
}
void payWidget::on_tempSave_clicked()
{
    for(;haveSaved<ui->historyDataList->count();haveSaved++){
        ui->Output_listWidget->addItem(ui->historyDataList->itemText(haveSaved));
        ui->Output_listWidget->item(haveSaved)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
    }
}
void payWidget::on_outputFile_clicked()
{
    QString dir= QFileDialog::getExistingDirectory(NULL,"选择保存位置",".");
    if(dir=="")
        return;
    for(int i=0;i<haveSaved;i++){
        if(flagOut[i]==1)
        {
            excel->exportData(dir+ui->Output_listWidget->item(i)->text(),*historyDataList[i]);
            ui->Output_listWidget->item(i)->setIcon(QIcon(":/icons/8666665_check.png"));
            flagOut[i]=3;
        }
    }
}
void payWidget::on_Output_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->sigle_outputFile->show();
    seletedOutputQListWidgetItem=item;
    if(flagOut[ui->Output_listWidget->row(item)]!=3){
        flagOut[ui->Output_listWidget->row(item)]=1;
        item->setIcon(QIcon(":/icons/8666666_square_check.png"));
    }
    ui->outputDelete_pushButton->setEnabled(true);
    ui->sigle_outputFile->setEnabled(true);
    ui->outputFile->setEnabled(true);
}
void payWidget::on_clear_output_clicked()
{
    ui->Output_listWidget->clear();
    haveSaved=0;
    memset(flagOut,0,sizeof (flagOut));
}
void payWidget::on_pushButton_clicked()
{
    this->close();
}
void payWidget::on_grab_clicked()
{
    QPixmap p = this->grab();
    QImage image = p.toImage();
    QString label="PayWidget-"+QDateTime::currentDateTime().toString("hh:MM:ss");
    qDebug()<<label;
    images[label]=p;
    image.save(QString(label));
}
void payWidget::on_refresh_clicked()
{
    ui->OutputImages_listWidget->clear();
    for (QMap<QString,QPixmap>::iterator iter=images.begin();iter!=images.end();iter++){
        ui->OutputImages_listWidget->addItem(iter.key());
    }
}

void payWidget::on_OutputImages_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->imageShow->clear();
    ui->imageShow->setPixmap(images[item->text()].scaled(ui->imageShow->width(),ui->imageShow->height()));
    OutputImages_listWidget_itemClicked_Flag=true;
}
bool payWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (OutputImages_listWidget_itemClicked_Flag&&obj == ui->imageShow)//指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

            if(mouseEvent->button() == Qt::LeftButton)
            {
                QWidget*fullImage=new QWidget;
                QVBoxLayout *layout = new QVBoxLayout;
                QLabel *fullLabel=new QLabel;
                QString imageName=ui->OutputImages_listWidget->currentItem()->text();

                fullLabel->setPixmap(images[imageName]);
                layout->addWidget(fullLabel);
                fullImage->setLayout(layout);
                fullImage->showFullScreen();
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return QWidget::eventFilter(obj, event);
    }
}

void payWidget::on_enableDateSort_clicked()
{
    enableDateSort = true;
}

void payWidget::on_enableMoneySort_clicked()
{
    enableMoneySort = true;
}

void payWidget::on_enableDateSort_down_clicked()
{
    enableDateSort_down = true;
}

void payWidget::on_enableMoneySort_down_clicked()
{
    enableMoneySort_down = true;
}

void payWidget::on_outputDelete_pushButton_clicked()
{
    if(flagOut[ui->Output_listWidget->row(seletedOutputQListWidgetItem)]!=3){
        flagOut[ui->Output_listWidget->row(seletedOutputQListWidgetItem)]=2;
        seletedOutputQListWidgetItem->setIcon(QIcon(":/icons/8666595_x.png"));
    }
}

void payWidget::on_sigle_outputFile_clicked()
{
    for(int i=0;i<haveSaved;i++){
        if(flagOut[i]==1){
            QString dir= QFileDialog::getExistingDirectory(NULL,QString("选择%1保存位置").arg(ui->Output_listWidget->item(i)->text()),".");
            if(dir=="")continue;
            excel->exportData(dir+ui->Output_listWidget->item(i)->text(),*historyDataList[i]);
            ui->Output_listWidget->item(i)->setIcon(QIcon(":/icons/8666665_check.png"));
            flagOut[i]=3;
        }
    }
}

void payWidget::on_message_stateChanged(int arg1)
{
    messageFlag=arg1;
}

void payWidget::on_importfile_5_clicked()
{
    for(int i=0;i<ui->OutputImages_listWidget->count();i++){
        QString imageName=ui->OutputImages_listWidget->item(i)->text();
        images[imageName].toImage().save(imageName.mid(0,5)+QString::number(i)+".png");
        ui->OutputImages_listWidget->item(i)->setIcon(QIcon(":/icons/8666665_check.png"));
    }
}
