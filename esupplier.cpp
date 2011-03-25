#include "esupplier.h"
#include "ui_esupplier.h"

#include "edbconnection.h"

ESupplier::ESupplier(EUser& user, QWidget *parent) :
    QWidget(parent),
    EUser(user),
    ui(new Ui::ESupplier)
{
    qDebug(": ESupplier >> Supplier [EUser]");

    QString query = QString("SELECT id, distance, company_name FROM suppliers WHERE user_id='%1'").arg(user.getID());
    QList<QStringList> List = EDBconnection::getInstance()->executeSelQuery(query);

    if (List.isEmpty()) {
        qDebug("EClient constructor error: List is empty");
    } else {
        id = List[0].at(0).toLong();
        distance = List[1].at(0).toLong();
        companyName = List[2].at(0);
        qDebug()<<"\nid :"<<id<<"\ndistnce :"<<distance<<"\ncompany name :"<<companyName;

        ui->setupUi(this);
        type_items = new QList<QTreeWidgetItem *>();
        items = new QList<QTreeWidgetItem *>();
        readData();
        on_treeWidget_itemClicked(type_items->at(0), 0);
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
    QString query = QString("SELECT rt.title as `type`, r.title as `title`, sr.price as `price`, sr.number as `number` " \
                            "FROM suppliers_resources AS sr " \
                            "INNER JOIN resources AS r ON sr.resource_id=r.id INNER JOIN resource_types AS rt ON r.resource_type_id=rt.id " \
                            "WHERE sr.supplier_id='%1' ORDER BY rt.title, r.title").arg(id);

    QList<QStringList> List = EDBconnection::getInstance()->executeSelQuery(query);

    if (type_items->length() > 0) type_items->clear();
    if (items->length() > 0) items->clear();
    ui->treeWidget->clear();
    type_items->append(new QTreeWidgetItem(ui->treeWidget, QStringList(QObject::tr("Add new"))));
    type_items->at(0)->setForeground(0, QBrush(QColor(0xFF, 0xAA, 0x00)));

    if (!List.isEmpty()) {
        for (int i=0; i < List.at(0).length(); ++i) {
            if (i == 0 || QString::compare(List.at(0).at(i), List.at(0).at(i-1), Qt::CaseSensitive) != 0) {
                type_items->append(new QTreeWidgetItem(ui->treeWidget, QStringList(List[0][i])));
                type_items->at(type_items->length()-1)->setForeground(0, QBrush(QColor(0x00, 0x2E, 0xB8)));
                type_items->at(type_items->length()-1)->setBackgroundColor(0, QColor(0xCC, 0xD9, 0xD3, 150));
                type_items->at(type_items->length()-1)->setBackgroundColor(1, QColor(0xCC, 0xD9, 0xD3, 150));
                type_items->at(type_items->length()-1)->setBackgroundColor(2, QColor(0xCC, 0xD9, 0xD3, 150));
            }

            QStringList tmpList;
            tmpList<<List[1][i]<<List[2][i]<<List[3][i];
            QTreeWidgetItem *temp_item = new QTreeWidgetItem(type_items->at(type_items->length() - 1), tmpList);
            items->append(temp_item);

            // public slots inherited from QTreeView  --  void QTreeView::expandAll () [slot]
            // Warning: if the model contains a large number of items, this function will take some time to execute.
            if (List[0].length() < 20) ui->treeWidget->expandAll();
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
    ui->update_add_btn->setEnabled(true);

    if (item->parent() != 0) {
        // resource selected
        ui->res_type_cb->setEditText(item->parent()->text(0));
        ui->res_name_cb->setEditText(item->text(0));
        ui->price_dspb->setValue(item->text(1).toDouble());
        ui->quantity_spb->setValue(item->text(2).toLong());
        ui->update_add_btn->setText(QString::fromUtf8("Изменить"));
        ui->delete_btn->setEnabled(true);
    } else {
        // resource type or 'Add New' selected
        ui->res_name_cb->clear();
        ui->res_type_cb->clear();
        ui->res_name_cb->setEditText("");
        ui->res_type_cb->setEditText("");
        ui->price_dspb->setValue(0.0);
        ui->quantity_spb->setValue(0);
        ui->delete_btn->setEnabled(false);

        if (ui->treeWidget->indexOfTopLevelItem(item) == 0) {
            // 'Add New' selected
            ui->update_add_btn->setText(QString::fromUtf8("Добавить"));

            QString query = QString("SELECT title FROM resource_types ORDER BY title");
            QList<QStringList> List = EDBconnection::getInstance()->executeSelQuery(query);

            ui->res_type_cb->addItem("");
            for (int i = 0; i < List[0].length(); ++i)  ui->res_type_cb->addItem(List[0].at(i));

        } else {
            // resource type selected
            ui->update_add_btn->setEnabled(false);
        }
    }
}

void ESupplier::on_delete_btn_clicked()
{
    // Delete resources
    qDebug("delete");
    QString res_type   = ui->res_type_cb->currentText();
    QString res_name = ui->res_name_cb->currentText();

    res_type.remove(QRegExp("['\"]"));
    res_name.remove(QRegExp("['\"]"));

    if (res_type.isEmpty() || res_name.isEmpty()) return;

   QString query = QString("DELETE FROM suppliers_resources WHERE " \
                          "(resource_id=(SELECT r.id FROM resources AS r JOIN resource_types AS rt WHERE rt.title='%1' AND r.title='%2') " \
                          "AND supplier_id='%3')").arg(res_type).arg(res_name).arg(id);
    EDBconnection::getInstance()->executeSqlQuery(query);

    readData();
}

void ESupplier::on_update_add_btn_clicked()
{
    // Add / Update resources info
    QString res_type   = ui->res_type_cb->currentText();
    QString res_name = ui->res_name_cb->currentText();
    double  price = ui->price_dspb->value();
    long      number = ui->quantity_spb->value();

    res_type.remove(QRegExp("['\"]"));
    res_name.remove(QRegExp("['\"]"));

    if (res_type.isEmpty() || res_name.isEmpty() || price == 0.0 || number == 0) return;

    // 'Add New' selected
    qDebug("Add new");
    // Insert resource type (UNIQUE)
    QString query = QString("INSERT INTO resource_types(title) VALUES('%1')").arg(res_type);
    EDBconnection::getInstance()->executeSqlQuery(query);

    // Insert resource (UNIQUE)
    query = QString("REPLACE INTO resources(title, resource_type_id) VALUES('%1', "
                    "(SELECT id FROM resource_types WHERE title='%2')) ").arg(res_name).arg(res_type);
    EDBconnection::getInstance()->executeSqlQuery(query);

    // Insert into suppliers_resources
    query = QString("REPLACE INTO suppliers_resources(resource_id, supplier_id, price, number) VALUES("
                    "(SELECT r.id FROM resources AS r JOIN resource_types AS rt WHERE rt.title='%1' AND r.title='%2'), '%3', '%4', '%5') ")
                .arg(res_type).arg(res_name).arg(id).arg(price).arg(number);
    EDBconnection::getInstance()->executeSqlQuery(query);

    readData();
}
