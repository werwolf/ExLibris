#ifndef ECLIENT_H
#define ECLIENT_H

#include <QWidget>
#include "euser.h"
#include "edbconnection.h"
#include "ui_eclient.h"

namespace Ui {
    class EClient;
}

class EClient :  public QWidget, public EUser
{
    Q_OBJECT

public:
    explicit EClient(EUser& user, QWidget *parent = 0);
    virtual ~EClient();

    long getClientID(void) const { return client_id; }
    QString getCompanyName (void) const { return companyName; }

    QWidget* getParent(void) { return this->window(); }

public slots:
//    void setClientInfo(QList<QStringList>);

private slots:
    void on_findServices_bt_clicked();
    void on_findResources_bt_clicked();
    void on_findQueries_bt_clicked();

    void on_tabs_currentChanged(int index = -1);

    void on_buy_btn_clicked();

    void on_report_btn_clicked();

//    void on_queryUpdate_btn_clicked();
//    void on_resourceUpdate_btn_clicked();
//    void on_serviceUpdate_btn_clicked();
//    void on_logUpdate_btn_clicked();

    void on_logFilter_cbx_currentIndexChanged(int index);

    void on_accEdit_btn_clicked();

    void on_company_edt_returnPressed();

    void selfDestruct();

private:
    Ui::EClient *ui;

    EDBconnection* db;
    long client_id;
    QString companyName;

    // private methods
    void readQueryData(QString cond = QString("1"));
    void readResourcesData(QString cond = QString("1"));
    void readServicesData(QString cond = QString("1"));
};

#endif // ECLIENT_H
