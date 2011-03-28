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
    virtual ~EClient();

public slots:
//    void setClientInfo(QList<QStringList>);

private slots:
    void on_findQueries_bt_clicked();

    void on_tabs_currentChanged(int index);

    void on_buy_btn_clicked();

private:
    Ui::EClient *ui;

    EDBconnection* db;
    long id;
    QString companyName;

    // private methods
    void readQueryData(QString cond = QString("1"));
    void readResourcesData(QString cond = QString("1"));
    void readServicesData(QString cond = QString("1"));
};

#endif // ECLIENT_H
