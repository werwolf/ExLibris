#include "eclient.h"
#include <QSpinBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>

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
    QString query = QString("SELECT * from queries_view WHERE %1 ").arg(cond);

    QList<QStringList> List = db->get(query);

    ui->query_tw->clearContents();
    ui->query_tw->setRowCount(List.length());

    for (int i = 0; i < List.length(); ++i) {
        ui->query_tw->setCellWidget(i, 6, new QSpinBox);
        ui->query_tw->setCellWidget(i, 0, new QCheckBox);


        for (int j = 0; j < List[0].length(); ++j) {
            ui->query_tw->setItem(i, j+1, new QTableWidgetItem(List[i].at(j)));
        }
    }

    ui->query_tw->setColumnWidth(0, 35);    // checkbox
    ui->query_tw->setColumnWidth(2, 100);   // title
    ui->query_tw->setColumnWidth(3, 100);   // author
    ui->query_tw->setColumnWidth(4, 80);    // genre
    ui->query_tw->setColumnWidth(5, 80);    // price
    ui->query_tw->setColumnWidth(6, 60);    // count
}

void EClient::readResourcesData(QString cond)
{
    QString query = QString("SELECT * FROM resources_view WHERE %1").arg(cond);

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

        QSpinBox *temp_spnb = qobject_cast<QSpinBox*>(ui->resource_tw->cellWidget(i, 6));
        temp_spnb->setMaximum(ui->resource_tw->item(i, 4)->text().toInt());
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
    QString query = QString("SELECT * from services_view WHERE %1").arg(cond);

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

    if (cond.isEmpty()) {
        readQueryData();
    } else {
        switch (ui->queryFilter_cbx->currentIndex()) {
        case 0:
            readQueryData(QString("title LIKE '%%1%'").arg(cond));
            break;

        case 1:
            readQueryData(QString("authors LIKE '%%1%'").arg(cond));
            break;

        case 2:
            readQueryData(QString("genre LIKE '%%1%'").arg(cond));
            break;

        case 3:
            readQueryData(QString("id LIKE '%%1%'").arg(cond));
            break;
        }
    }
}

void EClient::on_findResources_bt_clicked()
{
    // find
    QString cond = ui->resourceFilter_edt->text().trimmed();

    if (cond.isEmpty()) {
        readResourcesData();
    } else {
        switch (ui->resourceFilter_cbx->currentIndex()) {
        case 0:
            readResourcesData(QString("title LIKE '%%1%'").arg(cond));
            break;

        case 1:
            readResourcesData(QString("type LIKE '%%1%'").arg(cond));
            break;
        }
    }
}

void EClient::on_findServices_bt_clicked()
{
    // find
    QString cond = ui->serviceFilter_edt->text().trimmed();

    if (cond.isEmpty()) {
        readServicesData();
    } else {
        switch (ui->serviceFilter_cbx->currentIndex()) {
        case 0:
            readServicesData(QString("title LIKE '%%1%'").arg(cond));
            break;
        }
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
                QString book_id =ui->query_tw->item(i, 1)->text();
                qDebug()<<">>i :"<<i<<" book_id :"<<book_id;

                QSpinBox *temp_spnb = qobject_cast<QSpinBox*>(ui->query_tw->cellWidget(i, 6));
                long number = temp_spnb->value();
                qDebug()<<"number :"<<number;
                if (number == 0) continue;

                QString query = QString("INSERT INTO book_sell_log" \
                                            "(client_id,query_id,sum,number,income_date," \
                                            "start_print_date,end_print_date) " \
                                        "VALUES ("
                                            "'%1', '%2', getBookCost('%2'), '%3'," \
                                            "NOW(),NOW(),DATE_ADD(NOW(), INTERVAL 3 DAY)"
                                        ")").arg(this->getClientID()).arg(book_id).arg(number);
                if (db->insert(query) == -1) {
                    // show Error.
                    QMessageBox::warning(0, "Warning", trUtf8("Ошибка при совершении покупки."));
                    return;
                } else {
                    query = QString("SELECT resource_id, number FROM queries_resources WHERE query_id = '%1'")
                            .arg(book_id);
                    QList<QStringList> List = db->get(query);

                    // remove from stock (resources for printing)
                    for (int i = 0; i < List.length(); ++i) {
                        db->query(QString("UPDATE stock SET number = number - '%1' WHERE resource_id = '%2'")
                                .arg(List[i].at(1)).arg(List[i].at(0)));
                    }
                }
            }
        }
        break;

    case 1:
        if (ui->resource_tw->rowCount() == 0) return;

        for (int i = 0; i < ui->resource_tw->rowCount(); ++i) {
            QCheckBox *temp_chb = qobject_cast<QCheckBox*>(ui->resource_tw->cellWidget(i, 0));
            if (temp_chb->isChecked()) {
                QString resource_id = ui->resource_tw->item(i, 1)->text();
                qDebug()<<">>i :"<<i<<" res_id :"<<resource_id;

                QString price = ui->resource_tw->item(i, 5)->text();

                QSpinBox *temp_spnb = qobject_cast<QSpinBox*>(ui->resource_tw->cellWidget(i, 6));
                long number = temp_spnb->value();
                qDebug()<<"number :"<<number;
                if (number == 0) continue;

                QString query = QString("INSERT INTO resource_sell_log" \
                                                                 "(client_id,resource_id,sum,number,income_date,deal_date) " \
                                                             "VALUES ("
                                                                 "'%1', '%2', '%3', '%4', NOW(),DATE_ADD(NOW(), INTERVAL 1 DAY)"
                                                             ")").arg(this->getClientID()).arg(resource_id).arg(price).arg(number);

                if (db->insert(query) == -1) {
                    // show Error.
                    QMessageBox::warning(0, "Warning", trUtf8("Ошибка при совершении покупки."));
                    return;
                } else {
                    // remove from stock
                    db->query(QString("UPDATE stock SET number = number - '%1' WHERE resource_id = '%2'")
                            .arg(number).arg(resource_id));
                }
            }
        }
        break;

    case 2:
        if (ui->service_tw->rowCount() == 0) return;

        for (int i = 0; i < ui->service_tw->rowCount(); ++i) {
            QCheckBox *temp_chb = qobject_cast<QCheckBox*>(ui->service_tw->cellWidget(i, 0));
            if (temp_chb->isChecked()) {
                QString service_id = ui->service_tw->item(i, 1)->text();
                qDebug()<<">>i :"<<i<<" service_id :"<<service_id;

                QString price = ui->service_tw->item(i, 3)->text();

                QString query = QString("INSERT INTO service_sell_log" \
                                            "(client_id,service_id,sum,income_date,deal_date) " \
                                        "VALUES ("
                                            "'%1', '%2', '%3', NOW(),DATE_ADD(NOW(), INTERVAL 1 DAY)"
                                        ")").arg(this->getClientID()).arg(service_id).arg(price);
                if (db->insert(query) == -1) {
                    // show Error.
                    QMessageBox::warning(0, "Warning", trUtf8("Ошибка при совершении покупки."));
                    return;
                }
            }
        }
        break;
    }

    QMessageBox::information(0, "Congratulation", trUtf8("Покупка совершена."));
    on_tabs_currentChanged(tab_index);
}

void EClient::on_report_btn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, trUtf8("Save Report"), "", trUtf8("Save Reports (*.html)"));
    qDebug()<<"filename :"<<fileName;
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // show ERROR message
        QMessageBox::warning(0, "Warning", trUtf8("Ошибка.\nНельзя создать (открыть) файл."));
        return;
    }

    QString query;

    switch (ui->tabs->currentIndex()) {
    case 0 : query = QString("SELECT * from queries_view"); break;
    case 1 : query = QString("SELECT * from resources_view");  break;
    case 2 : query = QString("SELECT * from services_view");  break;
    }

    QList<QStringList> List = db->get(query);

     if (!List.isEmpty()) {
         QTextStream out(&file);

         out << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">";
         out << "<html> <head> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">";
         out << "<title>Report : 'resource_sell_log' table.</title> </head> <body>";

         out << "<p color=\"blue\">" + query + "</p>";

         // :TODO добавление названия колонок
         if (List.length() == 1 && List[0].length() == 1) {
             out << List[0].at(0);
         } else {
             QString html_table = "<table width=\"100%\" border=\"1\" cellspacing=\"0\" cellpadding=\"4\">";
             for (int i = 0; i < List.length(); ++i) {
                 QString row;
                 for (int j = 0; j < List[0].length(); ++j) {
                     row += "<td>" + List[i].at(j) + "</td>";
                 }
                 html_table += "<tr>"+row+"</tr>";
             }
             html_table += "</table>";

             out<<html_table;

             out << "</body> </html>";
         }
    } else {
         // show ERROR message
        QMessageBox::warning(0, "Warning", trUtf8("В таблице нет записей."));
     }
}
