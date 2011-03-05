#include "euser.h"
#include "edbconnection.h"
#include <QSqlQuery>

//EUser::EUser()
//{
//}

//void EUser::newUser(QString login,
//               QString password,
//               QString lasname,
//               QString name,
//               QString address,
//               QString phone,
//               QString email,
//               QString type,
//               QString regdate)
//void EUser::newUser()
//{
////    EDBconnection *db = EDBconnection::getInstance();
////    QSqlQuery sqlQuery(db);
////    sqlQuery.prepare("INSERT INTO users (id, login, password, lasname, name, address, phone, email, type, regdate)"
////        "VALUES (:id, :login, :password, :lasname, :name, :address, :phone, :email, :type, :regdate)");
////    sqlQuery.bindValue(":id", "NULL");
////    sqlQuery.bindValue(":login", login);
////    sqlQuery.bindValue(":password", password);
////    sqlQuery.bindValue(":lasname", lasname);
////    sqlQuery.bindValue(":name", name);
////    sqlQuery.bindValue(":address", address);
////    sqlQuery.bindValue(":phone", phone);
////    sqlQuery.bindValue(":email", email);
////    sqlQuery.bindValue(":type", type);
////    sqlQuery.bindValue(":regdate", regdate);

//    regdate = "";
//    QString query("INSERT INTO users (id, login, password, lasname, name, address, phone, email, type, regdate)"
//                  "VALUES (NULL, %1, %2, %3, %4, %5, %6, %7, %8, %9)");
//    query.arg(login).arg(password).arg(lasname).arg(name).arg(address).arg(phone).arg(email).arg(type).arg(regdate);
////    EDBconnection::executeSqlQuery(query);
////    emit addUser(query);
//}
