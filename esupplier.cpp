#include "esupplier.h"
#include "ui_esupplier.h"

ESupplier::ESupplier(EUser& user, QWidget *parent) :
    QWidget(parent),
    EUser(user),
    ui(new Ui::ESupplier)
{
    db = EDBconnection::getInstance();
    qDebug(": ESupplier >> Supplier [EUser]");

    QString query = QString("SELECT id, distance, company_name FROM suppliers WHERE user_id='%1'").arg(user.getID());
    QList<QStringList> List = db->get(query);

    if (List.isEmpty()) {
        qDebug("EClient constructor error: List is empty");
    } else {
        id = List[0].at(0).toLong();
        distance = List[0].at(1).toLong();
        companyName = List[0].at(2);
        qDebug()<<"\nid :"<<id<<"\ndistnce :"<<distance<<"\ncompany name :"<<companyName;

        ui->setupUi(this);
        type_items = new QList<QTreeWidgetItem *>();
        items = new QList<QTreeWidgetItem *>();

        // hide id column
        ui->treeWidget->hideColumn(3);

        readData();
        on_treeWidget_itemClicked(type_items->at(0), 0);
    }
}

ESupplier::~ESupplier()
{
    qDebug("ESupplier destructor.");
//    delete items;
//    delete type_items;
//    delete ui;
}

bool ESupplier::readData(void)
{
    QString query = QString("SELECT rt.title as `type`, r.title as `title`, sr.price as `price`, sr.number as `number`, r.id as `id` " \
                            "FROM suppliers_resources AS sr " \
                            "INNER JOIN resources AS r ON sr.resource_id=r.id INNER JOIN resource_types AS rt ON r.resource_type_id=rt.id " \
                            "WHERE sr.supplier_id='%1' ORDER BY rt.title, r.title").arg(id);

    QList<QStringList> List = db->get(query);

    if (type_items->length() > 0) type_items->clear();
    if (items->length() > 0) items->clear();
    ui->treeWidget->clear();
    type_items->append(new QTreeWidgetItem(ui->treeWidget, QStringList(QObject::tr("Add new"))));
    type_items->at(0)->setForeground(0, QBrush(QColor(0xFF, 0xAA, 0x00)));

    if (!List.isEmpty()) {
        for (int i=0; i < List.length(); ++i) {
            if (i == 0 || QString::compare(List.at(i).at(0), List.at(i-1).at(0), Qt::CaseSensitive) != 0) {
                type_items->append(new QTreeWidgetItem(ui->treeWidget, QStringList(List[i][0])));
                type_items->at(type_items->length()-1)->setForeground(0, QBrush(QColor(0x00, 0x2E, 0xB8)));
                type_items->at(type_items->length()-1)->setBackgroundColor(0, QColor(0xCC, 0xD9, 0xD3, 150));
                type_items->at(type_items->length()-1)->setBackgroundColor(1, QColor(0xCC, 0xD9, 0xD3, 150));
                type_items->at(type_items->length()-1)->setBackgroundColor(2, QColor(0xCC, 0xD9, 0xD3, 150));
            }

            QStringList tmpList;
            tmpList<<List[i][1]<<List[i][2]<<List[i][3]<<List[i][4];
            QTreeWidgetItem *temp_item = new QTreeWidgetItem(type_items->at(type_items->length() - 1), tmpList);
            items->append(temp_item);

            // public slots inherited from QTreeView  --  void QTreeView::expandAll () [slot]
            // Warning: if the model contains a large number of items, this function will take some time to execute.
            if (List.length() < 20) ui->treeWidget->expandAll();
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
        ui->res_type_cb->setEnabled(false);
        ui->res_name_cb->setEnabled(false);
    } else {
        // resource type or 'Add New' selected
        ui->res_name_cb->clear();
        ui->res_type_cb->clear();
        ui->res_name_cb->setEditText("");
        ui->res_type_cb->setEditText("");
        ui->price_dspb->setValue(0.0);
        ui->quantity_spb->setValue(0);
        ui->delete_btn->setEnabled(false);
        ui->res_type_cb->setEnabled(true);
        ui->res_name_cb->setEnabled(true);

        if (ui->treeWidget->indexOfTopLevelItem(item) == 0) {
            // 'Add New' selected
            ui->update_add_btn->setText(QString::fromUtf8("Добавить"));

            QString query = QString("SELECT title FROM resource_types ORDER BY title");
            QList<QStringList> List = db->get(query);

            ui->res_type_cb->addItem("");
            for (int i = 0; i < List.length(); ++i)  ui->res_type_cb->addItem(List[i].at(0));

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

    QString query = QString("DELETE FROM suppliers_resources WHERE resource_id='%1' AND supplier_id='%2'")
                            .arg(ui->treeWidget->currentItem()->text(3)).arg(id);
    db->query(query);

    // select 'add new' item
//    ui->treeWidget->setCurrentItem(ui->treeWidget->currentItem()->parent());

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

    if (ui->treeWidget->currentItem()->parent() != 0) {
        qDebug("Update");

        // Update suppliers_resources
        QString query = QString("UPDATE suppliers_resources SET price = '%1', number = '%2'" \
                                "WHERE resource_id = '%3' AND supplier_id = '%4' ")
                                .arg(price).arg(number).arg(ui->treeWidget->currentItem()->text(3)).arg(id);
        db->query(query);
    } else if (ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem()) == 0) {
        qDebug("Add new");

        // Insert resource type (UNIQUE)
        QString query = QString("INSERT INTO `resource_types` VALUES(NULL, '%1')").arg(res_type);
        int res_type_id = db->insert(query);

        if (res_type_id == -1) {
            // this resource type already exists
            query = QString("SELECT id FROM `resource_types` WHERE title = '%1'").arg(res_type);
            res_type_id = db->get(query)[0].at(0).toInt();
        }
        qDebug()<<"res_type_id :"<<res_type_id;

        // Insert resource (UNIQUE)
        query = QString("INSERT INTO resources(title, resource_type_id) VALUES('%1', '%2') ").arg(res_name).arg(res_type_id);
        int res_id = db->insert(query);

        if (res_id == -1) {
            // ERROR message
            return;
        }

        // Insert into suppliers_resources
        query = QString("INSERT INTO suppliers_resources (resource_id, supplier_id, price, number) VALUES('%1', '%2', '%3', '%4') ")
                .arg(res_id).arg(id).arg(price).arg(number);
        if (db->insert(query) == -1) {
            // ERROR message
            return;
        }
    }

    readData();
}
