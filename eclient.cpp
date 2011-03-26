#include "eclient.h"
#include "edbconnection.h"

//EClient::EClient(long _id, QWidget *parent) :
//  QWidget(parent),
//  EUser(_id),
//  ui(new Ui::EClientForm)
//{
//    qDebug(":TODO make EClient(long _id) constructor");
//    qDebug(": EClient >> Client [id]");
//    ui->setupUi(this);
//}

EClient::EClient(EUser& user, QWidget *parent) :
    QWidget(parent),
    EUser(user),
    ui(new Ui::EClient)
{
    qDebug(": EClient >> Client [EUser]");

    QString query = QString("SELECT company_name FROM clients WHERE user_id='%1'").arg(user.getID());
    QList<QStringList> List = EDBconnection::getInstance()->executeSelQuery(query);

    if (List.isEmpty()) {
        qDebug("EClient constructor error: List is empty");
    } else {
        companyName = List[0].at(0);
        qDebug()<<"company name :"<<companyName;

        ui->setupUi(this);
    }
}

EClient::~EClient()
{
    qDebug("EClient destructor");
    delete ui;
}
