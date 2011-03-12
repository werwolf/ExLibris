#ifndef ECLIENT_H
#define ECLIENT_H

#include "euser.h"

class EClient : public EUser
{
public:
//    EClient();
    explicit EClient(long _id);
    explicit EClient(EUser& );
//    ~EClient;

//signals:
//    void selectClient(QString);

public slots:
    void setClientInfo(QList<QStringList>);

private:
    QString companyName;
};

#endif // ECLIENT_H
