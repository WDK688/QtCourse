#include "idatabase.h"

void IDatabase::ininDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    QString aFile = "E:/QtCode/lab3/lab3.db";
    database.setDatabaseName(aFile);


    if (!database.open()) {
        qDebug() << "failed to open database";
    } else {
        qDebug() << "open database is ok";
    }
}

QString IDatabase::userLogin(QString userName, QString password)
{
    //return "loginOK";
    QSqlQuery query;
    query.prepare("select username, password from user where username = :USER");
    query.bindValue(":USER", userName);

    query.exec();

    if (query.first() && query.value("username").isValid()) {
        QString passwd = query.value("password").toString();
        if (passwd == password) {
            return "loginOk";
        } else {
            return "wrongPassword";
        }
    } else {
        qDebug() << "no such user: " << userName;
        return "wrongUsername";
    }
}


IDatabase::IDatabase(QObject *parent)
    : QObject{parent}
{
    ininDatabase();
}
