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
//    explicit EClient(long _id, QWidget *parent = 0);
    explicit EClient(EUser& user, QWidget *parent = 0);
//    explicit EClient( const EClient& );
//    EClient& operator=( const EClient& );
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
    void on_tabs_currentChanged(int index);
    void on_buy_btn_clicked();

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
