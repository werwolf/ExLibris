#include "eoperator.h"
#include "ui_eoperator.h"
#include <QSpinBox>
#include <QCheckBox>

EOperator::EOperator(EUser& user, QWidget* parent) :
    QWidget(parent),
    EUser(user),
    ui(new Ui::EOperator)
{
    db = EDBconnection::getInstance();
    qDebug(": EOperator >> Operator [EOperator]");

    ui->setupUi(this);

    readResourcesData();

    connect(ui->update_btn, SIGNAL(clicked()), this, SLOT(readResourcesData()));

    // res_types
    QString query = QString("SELECT title FROM resource_types ORDER BY title");
    QList<QStringList> List = db->get(query);
    ui->resourceType_cbx->clear();
    ui->resourceType_cbx->setEditText("");
    ui->resourceType_cbx->addItem(QString::fromUtf8("все"));
    for (int i = 0; i < List.length(); ++i)  ui->resourceType_cbx->addItem(List[i].at(0));
}

EOperator::~EOperator()
{
    delete ui;
}

void EOperator::readResourcesData(QString cond)
{
    QString query = QString("SELECT * FROM op_resources_view WHERE %1").arg(cond);

    QList<QStringList> List = db->get(query);

    ui->res_tw->clearContents();
    ui->res_tw->setRowCount(List.length());
    ui->res_tw->hideColumn(1);
    ui->res_tw->hideColumn(2);

    for (int i = 0; i < List.length(); ++i) {
        QSpinBox *temp_spnb = new QSpinBox;
        temp_spnb->setMaximum(List[i].at(6).toInt());

        ui->res_tw->setCellWidget(i, 9, temp_spnb);
        ui->res_tw->setCellWidget(i, 0, new QCheckBox);

        for (int j = 0; j < List[0].length(); ++j) {
            ui->res_tw->setItem(i, j+1, new QTableWidgetItem(List[i].at(j)));
        }
    }

    ui->res_tw->setColumnWidth(0, 35);

    // balance
    List = db->get(QString("SELECT currentBalance()"));
    ui->balance_lbl->setText(QString::fromUtf8("Баланс: ")+List[0].at(0));
}

void EOperator::on_resourceType_cbx_currentIndexChanged(QString res_type)
{
    if (res_type != QString::fromUtf8("все")) {
        readResourcesData(QString("type LIKE '%%1%'").arg(res_type));
    } else {
        readResourcesData();
    }
}

void EOperator::on_buy_resources_btn_clicked()
{
    for (int i = 0; i < ui->res_tw->rowCount(); ++i) {
        QCheckBox *temp_chb = qobject_cast<QCheckBox*>(ui->res_tw->cellWidget(i, 0));
        if (temp_chb->isChecked()) {
            QString resource_id = ui->res_tw->item(i, 1)->text();
            qDebug()<<">>i :"<<i<<" res_id :"<<resource_id;

            QString supplier_id = ui->res_tw->item(i, 2)->text();
            qDebug()<<"supplier_id :"<<supplier_id;

            QString price = ui->res_tw->item(i, 8)->text();

            QSpinBox *temp_spnb = qobject_cast<QSpinBox*>(ui->res_tw->cellWidget(i, 9));
            long number = temp_spnb->value();
            qDebug()<<"number :"<<number;
            if (number == 0) continue;

            QList<QStringList> List = db->get("SELECT id FROM garage LIMIT 1");
            QString garage_id = List[0].at(0);

            QString query = QString("INSERT INTO buy_log" \
                                        "(resource_id,supplier_id,garage_id,number," \
                                        "SUM,deal_date) " \
                                    "VALUES ('%1', '%2', '%3','%4', '%5', NOW())")
                            .arg(resource_id).arg(supplier_id).arg(garage_id).arg(number).arg(price);
            if (db->insert(query) == -1) {
                // show Error.
                return;
            } else {
                // add to stock
                db->query(QString("UPDATE stock SET number = number + '%1' WHERE resource_id = '%2'")
                        .arg(number).arg(resource_id));
                // remove from supplier
                db->query(QString("UPDATE suppliers_resources SET number = number - '%1' " \
                                  "WHERE supplier_id = '%2' AND resource_id = '%3'")
                        .arg(number).arg(supplier_id).arg(resource_id));
            }
        }
    }
    readResourcesData();
}

void EOperator::on_tabs_wg_currentChanged(int index)
{
    switch (index) {
    case 0 : readResourcesData(); break;
    case 2 : break;
    }
}
