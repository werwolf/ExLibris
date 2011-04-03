#include "esupplier.h"
#include "ui_esupplier.h"
#include <QFileDialog>
#include <QMessageBox>

ESupplier::ESupplier(EUser& user, QWidget *parent) :
    QWidget(parent),
    EUser(user),
    ui(new Ui::ESupplier)
{
    db = EDBconnection::getInstance();
    qDebug(": ESupplier >> Supplier [EUser]");

    QString query = QString("SELECT id, distance, company_name FROM suppliers WHERE user_id='%1'").arg(user.getUserID());
    QList<QStringList> List = db->get(query);

    if (List.isEmpty()) {
        qDebug("EClient constructor error: List is empty");
    } else {
        id = List[0].at(0).toLong();
        distance = List[0].at(1).toLong();
        companyName = List[0].at(2);
        qDebug()<<"\nid :"<<id<<"\ndistnce :"<<distance<<"\ncompany name :"<<companyName;

        ui->setupUi(this);
        type_items = new QList<QTreeWidgetItem *>;
        items = new QList<QTreeWidgetItem *>;

        // hide id column
        ui->treeWidget->hideColumn(3);

        readData();
        on_treeWidget_itemClicked(type_items->at(0), 0);

        connect(ui->update_btn, SIGNAL(clicked()), this, SLOT(readData()));
        connect(ui->self_destruct_btn, SIGNAL(clicked()), this, SLOT(selfDestruct()));
    }    
}

ESupplier::~ESupplier()
{
    qDebug("ESupplier destructor.");
    delete items;
    delete type_items;
    delete ui;
}

void ESupplier::selfDestruct()
{
    QString query = QString("SELECT id FROM buy_log WHERE deliver_date = '0000-00-00 00:00:00' " \
                            "AND supplier_id = '%1' LIMIT 1").arg(id);

    QList<QStringList> List = db->get(query);
    if (List.isEmpty()) {
        db->deleteUser(this->getUserID());
    } else {
        QMessageBox::warning(0, "Warning", trUtf8("Невозможно удалиться из системы!\n" \
                                                  "Дождитесь окончания всех перевозок."));
    }
}

bool ESupplier::readData(void)
{
    QString query = QString("SELECT `type`, `title`, `price`, `number`, `res_id`" \
                            " FROM suppliers_resources_view WHERE supplier_id='%1' ORDER BY type, title").arg(id);

    QList<QStringList> List = db->get(query);

    if (type_items->length() > 0) type_items->clear();
    if (items->length() > 0) items->clear();
    ui->treeWidget->clear();
    type_items->append(new QTreeWidgetItem(ui->treeWidget, QStringList(QObject::trUtf8("Add new"))));
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

    // select, click & focus 'add new' item
    ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
    on_treeWidget_itemClicked(ui->treeWidget->currentItem(), 0);
    ui->treeWidget->setFocus();

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
//        ui->res_type_cb->addItem(item->parent()->text(0));
//        ui->res_type_cb->addItem(item->text(0));
//        ui->res_type_cb->setEditable(false);
//        ui->res_name_cb->setEditable(false);
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

    QMessageBox msgBox;
    msgBox.setWindowTitle(trUtf8("Внимание"));
    msgBox.setInformativeText(trUtf8("Вы действительно хотите удалить этот ресурс?"));
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);

    if (msgBox.exec() == QMessageBox::No) return;

    QString query = QString("DELETE FROM suppliers_resources WHERE resource_id='%1' AND supplier_id='%2'")
                            .arg(ui->treeWidget->currentItem()->text(3)).arg(id);
    db->query(query);

    readData();
}

void ESupplier::on_update_add_btn_clicked()
{
    // Add / Update resources info
    QString res_type   = EDBconnection::escape(ui->res_type_cb->currentText());
    QString res_name = EDBconnection::escape(ui->res_name_cb->currentText());
    double  price = ui->price_dspb->value();
    long      number = ui->quantity_spb->value();

    if (res_type.isEmpty() || res_name.isEmpty() || price == 0.0 || number == 0) return;

    if (ui->treeWidget->currentItem()->parent() != 0) {
        qDebug("Update");

        // Update suppliers_resources
        QString query = QString("UPDATE suppliers_resources SET price = '%1', number = '%2' " \
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
            QMessageBox::warning(0, "Warning", trUtf8("Ошибка при добавлении ресурса."));
            return;
        }

        // Insert into suppliers_resources
        query = QString("INSERT INTO suppliers_resources (resource_id, supplier_id, price, number) VALUES('%1', '%2', '%3', '%4') ")
                .arg(res_id).arg(id).arg(price).arg(number);
        if (db->insert(query) == -1) {
            // ERROR message
            QMessageBox::warning(0, "Warning", trUtf8("Ошибка при занесении данных в БД."));
            return;
        }
    }

    readData();
}

void ESupplier::on_report_btn_clicked()
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

    QString query = QString("SELECT id, resource_id, supplier_id, garage_id, number, sum, deal_date, deliver_date " \
                                            "FROM `buy_log` WHERE supplier_id = '%1'").arg(this->id);
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
