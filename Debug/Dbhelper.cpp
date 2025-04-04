#include "DBHelper.h"
DBHelper* DBHelper::instance=0;
DBHelper* DBHelper::getInstance()
{
    if(0==instance)
        instance=new DBHelper();
    return instance;
}

DBHelper::DBHelper()
{
    db=QSqlDatabase::addDatabase("QSQLITE");
}

void DBHelper::creatConn()
{
    db.setDatabaseName("tel.db");
    db.open();
}

void DBHelper::destroyConn()
{
    db.close();
    db.removeDatabase("tel.db");
}
