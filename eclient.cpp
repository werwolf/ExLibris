#include "eclient.h"
#include "edbconnection.h"

EClient::EClient(long _id) : EUser(_id)
{
    qDebug(":TODO");
    qDebug(": EClient >> Client [id]");
}

EClient::EClient(EUser& user) : EUser(user)
{
    qDebug(": EClient >> Client [EUser]");

    QString query = QString("SELECT company_name FROM clients WHERE user_id='%1'").arg(user.getID());
    QList<QStringList> List = EDBconnection::getInstance()->executeSelQuery(query);

    if (List.isEmpty()) {
        qDebug("EClient constructor error: List is empty");
    } else {
        companyName = List[0].at(0);
        qDebug()<<"company name :"<<companyName;
    }
}

//virtual EClient::~EClient()
//{
//    qDebug("EClient destructor");
//}
