#include "euser.h"
#include "edbconnection.h"
#include <QDebug>

EUser::EUser(long user_id)
{
    QString query = QString("SELECT * FROM users WHERE id='%1'").arg(user_id);
//    QString query("SELECT * FROM users");

    connect(this, SIGNAL(selectUser(QString)), EDBconnection::getInstance(), SLOT(executeSelQuery(QString)));
    connect(EDBconnection::getInstance(), SIGNAL(setUserInfo(QList<QStringList>)), this, SLOT(setUserInfo(QList<QStringList>)));

    emit selectUser(query);
}

EUser::EUser(QString _login)
{
    Q_UNUSED(_login);
}

void EUser::setUserInfo(QList<QStringList> List)
{
    id = List[0].at(0).toLong();
    login = List[1].at(0);
//    password = List[2].at(0);
    lasname  = List[3].at(0);
    name = List[4].at(0);
    address = List[5].at(0);
    phone = List[6].at(0);
    email = List[7].at(0);

    QString str_type = List[8].at(0);
    if (str_type == "CLIENT"  ) type = CLIENT;
    if (str_type == "AUTHOR"  ) type = AUTHOR;
    if (str_type == "SUPPLIER") type = SUPPLIER;
    if (str_type == "OPERATOR") type = OPERATOR;
    if (str_type == "CEO"     ) type = CEO;
    if (str_type == "ADMIN"   ) type = ADMIN;

//    regdate = QDateTime::fromString(List[9].at(0), "yyyy-MM-ddTHH:mm:ss");

    qDebug()<<"\nid : "<<id<<"\nlogin : "<<login/*<<"\npassword : "<<password*/<<"\nlastname : "<<lasname<<"\nname : "<<name;
    qDebug()<<"address : "<<address<<"\nphone : "<<phone<<"\nemail : "<<email<<"\ntype : "<<str_type/*<<"\nregdate : "<<regdate*/;
    qDebug()<<"\ntype #"<<type<<"\n";
}
