#include "eclient.h"

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
    db = EDBconnection::getInstance();
    qDebug(": EClient >> Client [EUser]");

    QString query = QString("SELECT company_name FROM clients WHERE user_id='%1'")
                    .arg(db->escape(QString::number( user.getID() )));  // ono takoe nado? :)
    QList<QStringList> List = db->get(query);

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
