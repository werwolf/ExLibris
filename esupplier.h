#ifndef ESUPPLIER_H
#define ESUPPLIER_H

#include "euser.h"
#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
    class ESupplier;
}

class ESupplier :  public QWidget, public EUser
{
    Q_OBJECT

public:
    explicit ESupplier(EUser& user, QWidget *parent = 0);
    virtual ~ESupplier();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
    void on_delete_btn_clicked();

    void on_update_add_btn_clicked();

private:
    Ui::ESupplier *ui;

    QString companyName;
    long distance;
    long id;

    QList<QTreeWidgetItem *>* type_items;
    QList<QTreeWidgetItem *>* items;

    // methods
    bool readData(void);
};

#endif // ESUPPLIER_H
