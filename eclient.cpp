#include "eclient.h"
#include <QSpinBox>
#include <QCheckBox>

//EClient::EClient(long _id, QWidget *parent) :
//  QWidget(parent),
//  EUser(_id),
//  ui(new Ui::EClientForm)
//{
//    qDebug(":TODO make EClient(long _id) constructor");
//    qDebug(": EClient >> Client [id]");
//    ui->setupUi(this);
//}

//EClient::EClient(const EClient& rhs)/* : QWidget(rhs.parentWidget()), EUser(rhs.getUserID())*/
//{
//    operator =(rhs);
//}

//EClient& EClient::operator=( const EClient& rhs) : QWidget(rhs.parentWidget()), EUser(rhs.getUserID())
//{
//    if (this == &rhs) return *this;

//    client_id = rhs.client_id;
//    companyName = rhs.companyName;

//    this->setUserID( rhs.getUserID() );
//    this->setLogin( rhs.getLogin() );
//    this->setlastname( rhs.getlastname() );
//    this->setName( rhs.getName() );
//    this->setAddress( rhs.getAddress() );
//    this->setPhone( rhs.getPhone() );
//    this->setEmail( rhs.getEmail() );
//    this->setType( rhs.getType() );
////    qDebug()<<login;
//    return *this;
//}

EClient::EClient(EUser& user, QWidget *parent) :
    QWidget(parent),
    EUser(user),
    ui(new Ui::EClient)
{
    db = EDBconnection::getInstance();
    qDebug(": EClient >> Client [EUser]");

    QString query = QString("SELECT id, company_name FROM clients WHERE user_id='%1'").arg(this->getUserID());
    QList<QStringList> List = db->get(query);

    if (List.isEmpty()) {
        qDebug("EClient constructor error: List is empty");
    } else {
        client_id = List[0].at(0).toInt();
        companyName = List[0].at(1);
        qDebug()<<"Client ID :"<<client_id<<"company name :"<<companyName;

        ui->setupUi(this);
    }


//    readQueryData();
    switch (ui->tabs->currentIndex()) {
    case 0:
        readQueryData();
        break;

    case 1:
        readResourcesData();
        break;

    case 2:
        readServicesData();
        break;
    }
}

EClient::~EClient()
{
    qDebug("EClient destructor");
    delete ui;
}

void EClient::readQueryData(QString cond)
{
    // :TODO: getBookPrice()
    QString query = QString("SELECT q.id, q.title, " \
                            "GROUP_CONCAT(DISTINCT CONCAT(u.lastname, ' ', u.name) ORDER BY u.lastname SEPARATOR ', '),  " \
                            "100, 0 " \
                            "FROM queries as q " \
                            "INNER JOIN genres as g ON g.id = q.genre_id " \
                            "INNER JOIN authors_queries as aq ON q.id = aq.query_id " \
                            "INNER JOIN authors as a ON aq.author_id = a.id " \
                            "INNER JOIN users as u ON u.id = a.user_id " \
                            "WHERE %1 " \
                            "GROUP BY q.id").arg(cond);

    QList<QStringList> List = db->get(query);

    ui->query_tw->clearContents();
    ui->query_tw->setRowCount(List.length());

    for (int i = 0; i < List.length(); ++i) {
        ui->query_tw->setCellWidget(i, 5, new QSpinBox);
        ui->query_tw->setCellWidget(i, 0, new QCheckBox);

        for (int j = 0; j < List[0].length(); ++j) {
            ui->query_tw->setItem(i, j+1, new QTableWidgetItem(List[i].at(j)));
        }
    }

    ui->query_tw->setColumnWidth(0, 35);    // checkbox
    ui->query_tw->setColumnWidth(2, 130);   // title
    ui->query_tw->setColumnWidth(3, 120);   // author
    ui->query_tw->setColumnWidth(4, 80);    // price
    ui->query_tw->setColumnWidth(5, 60);    // count
}

void EClient::readResourcesData(QString cond)
{
    QString query = QString("SELECT r.id, rt.title, r.title, s.number, r.price, 0 " \
                            "FROM resources as r " \
                            "INNER JOIN stock as s ON s.resource_id = r.id " \
                            "INNER JOIN resource_types as rt ON rt.id = r.resource_type_id " \
                            "WHERE %1").arg(cond);

    QList<QStringList> List = db->get(query);

    ui->resource_tw->clearContents();
    ui->resource_tw->setRowCount(List.length());
    ui->resource_tw->hideColumn(1);

    for (int i = 0; i < List.length(); ++i) {
        ui->resource_tw->setCellWidget(i, 6, new QSpinBox);
        ui->resource_tw->setCellWidget(i, 0, new QCheckBox);

        for (int j = 0; j < List[0].length(); ++j) {
            ui->resource_tw->setItem(i, j+1, new QTableWidgetItem(List[i].at(j)));
        }
    }

    ui->resource_tw->setColumnWidth(0, 35);
    ui->resource_tw->setColumnWidth(2, 90);
    ui->resource_tw->setColumnWidth(3, 120);
    ui->resource_tw->setColumnWidth(4, 100);
    ui->resource_tw->setColumnWidth(5, 100);
    ui->resource_tw->setColumnWidth(6, 60); // count
}

void EClient::readServicesData(QString cond)
{
    QString query = QString("SELECT s.id, s.title, s.price " \
                            "FROM services AS s " \
                            "INNER JOIN personnel_services AS ps ON ps.service_id = s.id " \
                            "INNER JOIN personnel AS p ON ps.personnel_id = p.id " \
                            "WHERE %1").arg(cond);

    QList<QStringList> List = db->get(query);

    ui->service_tw->clearContents();
    ui->service_tw->setRowCount(List.length());
    ui->service_tw->hideColumn(1);

    for (int i = 0; i < List.length(); ++i) {
        ui->service_tw->setCellWidget(i, 0, new QCheckBox);

        for (int j = 0; j < List[0].length(); ++j) {
            ui->service_tw->setItem(i, j+1, new QTableWidgetItem(List[i].at(j)));
        }
    }

    ui->service_tw->setColumnWidth(0, 35);
    ui->service_tw->setColumnWidth(2, 250);
    ui->service_tw->setColumnWidth(3, 100);
}

void EClient::on_findQueries_bt_clicked()
{
    // find
    QString cond = ui->queryFilter_edt->text().trimmed();
    if (cond.isEmpty()) readQueryData();

    switch (ui->queryFilter_cbx->currentIndex()) {
    case 0:
        readQueryData(QString("q.title LIKE '%%1%'").arg(cond));
        break;

    case 1:
        readQueryData(QString("(u.type = 'AUTHOR') AND ((u.name LIKE '%%1%') OR (u.lastname LIKE '%%1%'))").arg(cond));
        break;

    case 2:
        readQueryData(QString("g.title LIKE '%%1%'").arg(cond));
        break;

    case 3:
        readQueryData(QString("q.id LIKE '%%1%'").arg(cond));
        break;
    }
}

void EClient::on_findResources_bt_clicked()
{
    // find
    QString cond = ui->resourceFilter_edt->text().trimmed();
    if (cond.isEmpty()) readResourcesData();

    switch (ui->resourceFilter_cbx->currentIndex()) {
    case 0:
        readResourcesData(QString("r.title LIKE '%%1%'").arg(cond));
        break;

    case 1:
        readResourcesData(QString("rt.title LIKE '%%1%'").arg(cond));
        break;
    }
}

void EClient::on_findServices_bt_clicked()
{
    // find
    QString cond = ui->serviceFilter_edt->text().trimmed();
    if (cond.isEmpty()) readResourcesData();

    switch (ui->serviceFilter_cbx->currentIndex()) {
    case 0:
        readServicesData(QString("s.title LIKE '%%1%'").arg(cond));
        break;
    }
}

void EClient::on_tabs_currentChanged(int index)
{
    switch (index) {
    case 0:
        on_findQueries_bt_clicked();
        break;

    case 1:
        on_findResources_bt_clicked();
        break;

    case 2:
        on_findServices_bt_clicked();
        break;
    }
}

void EClient::on_buy_btn_clicked()
{
    int tab_index = ui->tabs->currentIndex();

    switch (tab_index) {
    case 0:
        if (ui->query_tw->rowCount() == 0) return;

        for (int i = 0; i < ui->query_tw->rowCount(); ++i) {
            QCheckBox *temp_chb = qobject_cast<QCheckBox*>(ui->query_tw->cellWidget(i, 0));
            if (temp_chb->isChecked()) {
                QString book_id =ui->query_tw->item(i, 5)->text();
                qDebug()<<">>i :"<<i<<" book_id :"<<book_id;

                QSpinBox *temp_spnb = qobject_cast<QSpinBox*>(ui->query_tw->cellWidget(i, 4));
                long number = temp_spnb->value();

                qDebug()<<"number :"<<number;
//                QString query = QString("%1").arg(number);
//                if (db->insert(query) == -1) {
//                    // show Error.
//                }
            }
        }
        break;
    case 1:
        if (ui->resource_tw->rowCount() == 0) return;

        break;
    case 2:
        if (ui->service_tw->rowCount() == 0) return;

        break;
    }

    on_tabs_currentChanged(tab_index);
}
