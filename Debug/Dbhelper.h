#ifndef DBHELPER_H
#define DBHELPER_H

#include <QSqlDatabase>
class DBHelper
{
public:
    static DBHelper* getInstance();
    void creatConn();
    void destroyConn();
private:
    QSqlDatabase db;
    static DBHelper* instance;
    DBHelper();
};

#endif // DBHELPER_H
