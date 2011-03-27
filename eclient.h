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

private:
    Ui::EClient *ui;

    EDBconnection* db;
    QString companyName;
};

#endif // ECLIENT_H
