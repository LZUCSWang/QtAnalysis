#include "excelengine.h"
#include <QDebug>
#include <QDateTime>
#include <QDir>
int ExcelEngine::ReadExcel(const QString& filePath,QVariantList &dataList)
{
    QAxObject *excel = new QAxObject("ket.Application");
    excel->setControl("Excel.Application");
    excel->setProperty("Visible", false);       // 不显示窗体
    excel->setProperty("DisplayAlerts", false); // 不显示任何警告信息。如果为true, 那么关闭时会出现类似"文件已修改，是否保存"的提示
    QAxObject* workbooks = excel->querySubObject("WorkBooks");  //获取工作簿集合
    workbooks->dynamicCall("Open(const QString&)", filePath); //打开打开已存在的工作簿
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook"); //获取当前工作簿
    QAxObject* sheet = workbook->querySubObject("WorkSheets(int)", 1);//获取工作表集合的工作表1，即sheet1
    QAxObject* range = sheet->querySubObject("UsedRange"); //获取该sheet的使用范围对象
    QVariant var = range->dynamicCall("Value");
    delete range;
    dataList.append(var.toList());  //得到表格中的所有数据
    dataList.pop_front();
    workbooks->dynamicCall("Close()"); //关闭工作簿
    excel->dynamicCall("Quit()"); //退出进程

    if (dataList.isEmpty()) {
        return -1;
    }
    return dataList.count();
}
QString ExcelEngine::exportData(QString lx,QVariantList &datalist)
{
    //流程：
    //1.新建一个excel
    //2.增加1个Worksheet
    //3.向Excel单元格中写入数据
    //4.保存Excel
    //5.释放Excel

    QString dtStr=lx+".xlsx";
    newExcel(dtStr);

    // appendSheet("车检数据",1);

    //新建数据的步骤：
    //1、标题
    //2、内容

    //设置标题
    setCellValue(1,1,"接收者");
    setCellValue(1,2,"发送者");
    setCellValue(1,3,"金额");
    setCellValue(1,4,"时间");


    //设置内容
    for(int i=0;i<datalist.size();i++){
        for (int j=0;j<datalist[0].toList().size();j++){
            setCellValue(i+2,j+1,datalist[i].toList()[j].toString());
        }
    }
    saveExcel(dtStr);
    freeExcel();
    return dtStr;
}
void ExcelEngine::newExcel(const QString &fileName)
{
    pApplication = new QAxObject();
    pApplication->setControl("Excel.Application");//连接Excel控件
    pApplication->dynamicCall("SetVisible(bool)", false);//false不显示窗体
    pApplication->setProperty("DisplayAlerts", false);//不显示任何警告信息。
    pWorkBooks = pApplication->querySubObject("Workbooks");
    QFile file(fileName);
    file.remove();
    pWorkBooks->dynamicCall("Add");//新建一个工作簿,如果不新建会崩溃
    pWorkBook = pApplication->querySubObject("ActiveWorkBook");//获取当前工作簿
    pSheets = pWorkBook->querySubObject("Sheets");//获取所有工作表sheet

    pSheet = pSheets->querySubObject("Item(int)", 1);//获取第一个工作表sheet，从1开始
}
void ExcelEngine::appendSheet(const QString &sheetName,int cnt)
{
    QAxObject *pLastSheet = pSheets->querySubObject("Item(int)", cnt);
    pSheets->querySubObject("Add(QVariant)", pLastSheet->asVariant());
    pSheet = pSheets->querySubObject("Item(int)", cnt);
    pLastSheet->dynamicCall("Move(QVariant)", pSheet->asVariant());
    pSheet->setProperty("Name", sheetName);
}
void ExcelEngine::setCellValue(int row, int column, const QString &value)
{   if(column!=8)
    {
        QAxObject *pRange = pSheet->querySubObject("Cells(int,int)", row, column);
        pRange->dynamicCall("Value", value);
    }
    else
    {
        QAxObject *pRange = pSheet->querySubObject("Cells(int,int)", row, column);
        pRange->dynamicCall("NumberFormatLocal","yyyy-mm-dd HH::mm:ss");//当为日期时间型数据时，按此格式设置
        pRange->dynamicCall("Value", value);
    }
}
void ExcelEngine::saveExcel(const QString &fileName)
{
    pWorkBook->dynamicCall("SaveAs(const QString &)", QDir::toNativeSeparators(fileName));
}
void  ExcelEngine::freeExcel()
{
    if (pApplication != NULL)
    {
        pApplication->dynamicCall("Quit()");
        delete pApplication;
        pApplication = NULL;
    }
}
