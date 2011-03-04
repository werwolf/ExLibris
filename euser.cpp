#include "euser.h"

EUser::EUser()
{
}

static void newUser(//QString id,
               QString login,
               QString password,
               QString lasname,
               QString name,
               QString address,
               QString phone,
               QString email,
               //QString type,
               QString regdate)
{
        query.prepare("INSERT INTO users (id, login, password, lasname, name, address, phone, email, type, regdate)"
        "VALUES (:id, :login, :password, :lasname, :name, :address, :phone, :email, :type, :regdate)");
        query.bindValue(":id", "NULL");
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.bindValue(":lasname", lasname);
        query.bindValue(":name", name);
        query.bindValue(":address", address);
        query.bindValue(":phone", phone);
        query.bindValue(":email", email);
        query.bindValue(":type", type);
        query.bindValue(":regdate", regdate);
}
