#ifndef ESUPPLIER_H
#define ESUPPLIER_H

#include "euser.h"
#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
    class ESupplier;
}

class ESupplier : public EUser, public QWidget
{
    Q_OBJECT

public:
    explicit ESupplier(QWidget *parent = 0);
    ~ESupplier();

private:
    Ui::ESupplier *ui;

//    QList<QTreeWidgetItem *> type_items;
//    QList<QTreeWidgetItem *> items;

    // methods
    bool readData(void);
};

#endif // ESUPPLIER_H
