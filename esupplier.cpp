#include "esupplier.h"
#include "ui_esupplier.h"

#include "edbconnection.h"

ESupplier::ESupplier(EUser& user, QWidget *parent) :
    QWidget(parent),
    EUser(user),
    ui(new Ui::ESupplier)
{
    qDebug(": ESupplier >> Supplier [EUser]");

    QString query = QString("SELECT distance, company_name FROM suppliers WHERE user_id='%1'").arg(user.getID());
    QList<QStringList> List = EDBconnection::getInstance()->executeSelQuery(query);

    if (List.isEmpty()) {
        qDebug("EClient constructor error: List is empty");
    } else {
        distnce = List[0].at(0).toLong();
        companyName = List[1].at(0);
        qDebug()<<"distnce :"<<distnce<<"\ncompany name :"<<companyName;

        ui->setupUi(this);
        type_items = new QList<QTreeWidgetItem *>();
        items = new QList<QTreeWidgetItem *>();
        readData();
    }
}

ESupplier::~ESupplier()
{
    qDebug("ESupplier destructor.");
    delete items;
    delete type_items;
    delete ui;
}

bool ESupplier::readData(void)
{
    QString query = "SELECT resource_types.title as type, resources.title as title, resources.price as price FROM resources " \
            "INNER JOIN resource_types ON resources.resource_type_id=resource_types.id ORDER BY resource_types.title";

    QList<QStringList> List = EDBconnection::getInstance()->executeSelQuery(query);

    if (!List.isEmpty()) {
        if (type_items->length() > 0) type_items->clear();
        if (items->length() > 0) items->clear();
        ui->treeWidget->clear();
        type_items->append(new QTreeWidgetItem(ui->treeWidget, QStringList(QObject::tr("Add new"))));

        for (int i=0; i < List.at(0).length(); ++i) {
            if (i == 0 || QString::compare(List.at(0).at(i), List.at(0).at(i-1), Qt::CaseSensitive) != 0)
                type_items->append(new QTreeWidgetItem(ui->treeWidget, QStringList(List[0][i])));

            QStringList tmpList;
            tmpList<<List[1][i]<<List[2][i];
            QTreeWidgetItem *temp_item = new QTreeWidgetItem(type_items->at(type_items->length() - 1), tmpList);
            items->append(temp_item);
        }
    } else {
        qDebug("List is empty");
        return false;
    }
    return true;
}

void ESupplier::on_treeWidget_itemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    if (item->parent() != 0) {
        ui->res_type_cb->setEditText(item->parent()->text(0));
        ui->res_name_cb->setEditText(item->text(0));
        ui->price_dspb->setValue(item->text(1).toDouble());
        ui->delete_btn->setEnabled(true);
        ui->update_add_btn->setText(tr("Изменить"));
    } else if (ui->treeWidget->indexOfTopLevelItem(item) == 0) {
        ui->res_type_cb->setEditText("");
        ui->res_name_cb->setEditText("");
        ui->price_dspb->setValue(0.0);
        ui->delete_btn->setEnabled(false);
        ui->update_add_btn->setText(tr("Добавить"));
    }
}

void ESupplier::on_delete_btn_clicked()
{
    // Delete resources
    qDebug("delete");
}

void ESupplier::on_update_add_btn_clicked()
{
    // Add / Update resources info
    qDebug("add/update");
}
