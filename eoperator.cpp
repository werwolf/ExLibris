#include "eoperator.h"
#include "ui_eoperator.h"

EOperator::EOperator(EUser& user, QWidget* parent) :
    QWidget(parent),
    EUser(user),
    ui(new Ui::EOperator)
{
//    db = EDBconnection::getInstance();
//    qDebug(": ESupplier >> Supplier [EUser]");

//    QString query = QString("SELECT id, distance, company_name FROM suppliers WHERE user_id='%1'").arg(user.getUserID());
//    QList<QStringList> List = db->get(query);

//    if (List.isEmpty()) {
//        qDebug("EClient constructor error: List is empty");
//    } else {
//        id = List[0].at(0).toLong();
//        distance = List[0].at(1).toLong();
//        companyName = List[0].at(2);
//        qDebug()<<"\nid :"<<id<<"\ndistnce :"<<distance<<"\ncompany name :"<<companyName;

//        ui->setupUi(this);
//        type_items = new QList<QTreeWidgetItem *>();
//        items = new QList<QTreeWidgetItem *>();

//        // hide id column
//        ui->treeWidget->hideColumn(3);

//        readData();
//        on_treeWidget_itemClicked(type_items->at(0), 0);
//    }
    ui->setupUi(this);
}

EOperator::~EOperator()
{
    delete ui;
}
