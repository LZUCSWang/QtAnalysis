#ifndef _EXCELENGINE_H_ 
#define _EXCELENGINE_H_ 
#include <QWidget>
#include <QAxObject>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
class ExcelEngine:public QWidget
{
    Q_OBJECT
public:
    int ReadExcel(const QString& filePath,QVariantList &rowData);
    QString exportData(QString lx,QVariantList&datalist);
    void newExcel(const QString &fileName);
    void appendSheet(const QString &sheetName,int cnt);
    void setCellValue(int row, int column, const QString &value);
    void saveExcel(const QString &fileName);
    void  freeExcel();
    QAxObject *pApplication,*pWorkBooks,*pWorkBook,*pSheets,*pSheet;
};

#endif  //EXCELENGINE
