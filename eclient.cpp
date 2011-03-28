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

EClient::EClient(EUser& user, QWidget *parent) :
    QWidget(parent),
    EUser(user),
    ui(new Ui::EClient)
{
    db = EDBconnection::getInstance();
    qDebug(": EClient >> Client [EUser]");

    QString query = QString("SELECT id, company_name FROM clients WHERE user_id='%1'")
                    .arg(db->escape(QString::number( user.getID() )));  // ono takoe nado? :)
    QList<QStringList> List = db->get(query);

    if (List.isEmpty()) {
        qDebug("EClient constructor error: List is empty");
    } else {
        id = List[0].at(0).toInt();
        companyName = List[0].at(1);
        qDebug()<<"Client ID :"<<id<<"company name :"<<companyName;

        ui->setupUi(this);
    }

//    ui->query_tw->hideColumn(5);
    readQueryData();
}

EClient::~EClient()
{
    qDebug("EClient destructor");
    delete ui;
}

void EClient::readQueryData(QString cond)
{
    QString query = QString("SELECT q.title, CONCAT(u.lastname, ' ', u.name), 100, 0, q.id " \
                            "FROM queries as q " \
                            "INNER JOIN genres as g ON g.id = q.genre_id " \
                            "INNER JOIN authors_queries as aq ON q.id = aq.query_id " \
                            "INNER JOIN authors as a ON aq.author_id = a.id " \
                            "INNER JOIN users as u ON u.id = a.user_id " \
                            "WHERE %1").arg(cond);

    QList<QStringList> List = db->get(query);

    ui->query_tw->clearContents();
    ui->query_tw->setRowCount(List.length());

    for (int i = 0; i < List.length(); ++i) {
        ui->query_tw->setCellWidget(i, 4, new QSpinBox);
        ui->query_tw->setCellWidget(i, 0, new QCheckBox);

        for (int j = 0; j < List[0].length(); ++j) {
            ui->query_tw->setItem(i, j+1, new QTableWidgetItem(List[i].at(j)));
        }
    }
}

void EClient::readResourcesData(QString cond)
{
    Q_UNUSED(cond);
}

void EClient::readServicesData(QString cond)
{
    Q_UNUSED(cond);
}

void EClient::on_findQueries_bt_clicked()
{
    // find
    QString cond = ui->queryFilter_edt->text();
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

void EClient::on_tabs_currentChanged(int index)
{
    switch (index) {
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

void EClient::on_buy_btn_clicked()
{
    int tab_index = ui->tabs->currentIndex();

    switch (tab_index) {
    case 0:

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

        break;
    case 2:

        break;
    }

    on_tabs_currentChanged(tab_index);
}
