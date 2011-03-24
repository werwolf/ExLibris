#include "esupplier.h"
#include "ui_esupplier.h"

#include "edbconnection.h"

ESupplier::ESupplier(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ESupplier)
{
    ui->setupUi(this);
    readData();
}

ESupplier::~ESupplier()
{
    delete ui;
}

bool ESupplier::readData(void)
{
    QString query = "SELECT resource_types.title as type, resources.title as title, resources.price as price FROM resources " \
            "INNER JOIN resource_types ON resources.resource_type_id=resource_types.id ORDER BY resource_types.title";

    QList<QStringList> List = EDBconnection::getInstance()->executeSelQuery(query);

    if (!List.isEmpty()) {
        QList<QTreeWidgetItem *> type_items;
        QList<QTreeWidgetItem *> items;
//        items.clear();
//        type_items.clear();
        int j = -1;
        for (int i=0; i < List.at(0).length(); ++i) {
            if (i == 0 || QString::compare(List.at(0).at(i), List.at(0).at(i-1), Qt::CaseSensitive) != 0) {
                QTreeWidgetItem *temp_item = new QTreeWidgetItem(ui->treeWidget);
                temp_item->setText(0, List[0][i]);
                type_items.append(temp_item);
                ++j;
            }
            QStringList tmpList ;
            tmpList<<List[1][i]<<List[2][i];
            QTreeWidgetItem *temp_item = new QTreeWidgetItem(type_items.at(j), tmpList);
            items.append(temp_item);
        }
    } else {
        qDebug("List is empty");
        return false;
    }
    return true;
}
