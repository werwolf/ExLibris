#include "euser.h"
#include <QDebug>

EUser::EUser(const EUser& rhs)// : QObject()
{
    operator =(rhs);
}

EUser& EUser::operator=( const EUser& rhs)
{
    if (this == &rhs) return *this;
    id = rhs.id;
    login = rhs.login;
    lastname  = rhs.lastname;
    name = rhs.name;
    address = rhs.address;
    phone = rhs.phone;
    email = rhs.email;
    type = rhs.type;
//    qDebug()<<login;
    return *this;
}

EUser::EUser(long user_id)
{
    db = EDBconnection::getInstance();
    QString query = QString("SELECT " \
                            "id, login, password, lastname, name, address, phone, email, type, reg_date " \
                            "FROM users WHERE id='%1'").arg(user_id);
    QList<QStringList> List = db->get(query);

    if (List.isEmpty()/* || List[0].isEmpty()*/) {
        qDebug("EUser constructor error: List is empty");
    } else {
        setUserInfo(List);
    }
}

EUser::EUser(QString _login)
{
    Q_UNUSED(_login);
    qDebug(":TODO");
}

EUser::~EUser() {
    qDebug("EUser destructor");
}

void EUser::setUserInfo(QList<QStringList> info)
{
    id = info[0].at(0).toLong();
    login = info[0].at(1);
//    password = info[0].at(2);
    lastname  = info[0].at(3);
    name = info[0].at(4);
    address = info[0].at(5);
    phone = info[0].at(6);
    email = info[0].at(7);

    QString str_type = info[0].at(8);
    if (str_type == "CLIENT"  ) type = CLIENT;
    if (str_type == "AUTHOR"  ) type = AUTHOR;
    if (str_type == "SUPPLIER") type = SUPPLIER;
    if (str_type == "OPERATOR") type = OPERATOR;
    if (str_type == "CEO"     ) type = CEO;
    if (str_type == "ADMIN"   ) type = ADMIN;

//    regdate = QDateTime::fromString(info[0].at(9), "yyyy-MM-ddTHH:mm:ss");

    qDebug()<<"\nid : "<<id<<"\nlogin : "<<login/*<<"\npassword : "<<password*/<<"\nlastname : "<<lastname<<"\nname : "<<name;
    qDebug()<<"address : "<<address<<"\nphone : "<<phone<<"\nemail : "<<email<<"\ntype : "<<str_type/*<<"\nregdate : "<<regdate*/;
    qDebug("type #%d\n",type);
}
