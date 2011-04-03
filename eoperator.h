#ifndef EOPERATOR_H
#define EOPERATOR_H

#include <QTabWidget>
#include "euser.h"

namespace Ui {
    class EOperator;
}

class EOperator : public QWidget, public EUser
{
    Q_OBJECT

public:
    explicit EOperator(EUser& user, QWidget* parent = 0);
    ~EOperator();

    long getOperatorID(void) const { return this->getUserID(); }

private:
    Ui::EOperator *ui;
    EDBconnection* db;

    // private methods
//    void readResourcesData(QString cond = "1");

private slots:
    void on_buy_resources_btn_clicked();
    void on_resourceType_cbx_currentIndexChanged(QString );

    void readResourcesData(QString cond = "1");
    void on_tabs_wg_currentChanged(int index);
};

#endif // EOPERATOR_H
