#ifndef EUSER_H
#define EUSER_H

#include <QtCore>
#include "edbconnection.h"

class EUser
{
public:
    enum UserType {CLIENT, AUTHOR, SUPPLIER, OPERATOR, CEO, ADMIN};

    explicit EUser(long _id);
    explicit EUser(QString _login);
    explicit EUser( const EUser& );
    EUser& operator=( const EUser& );
    virtual  ~EUser();

    long getID(void) const { return user_id; }
    QString getLogin (void) const { return login; }
    QString getlastname (void) const { return lastname; }
    QString getName (void) const { return name; }
    QString getAddress (void) const { return address; }
    QString getPhone (void) const { return phone; }
    QString getEmail (void) const { return email; }
    UserType getType(void ) const { return type; }

//public slots:
    void setUserInfo(QList<QStringList>);

//protected:
//    EUser( const EUser& );
//    EUser& operator=( const EUser& );

private:
    long    user_id;
    QString login;
//    QString password;
    QString lastname;
    QString name;
    QString address;
    QString phone;
    QString email;
    UserType  type;
    EDBconnection* db;
//    QDateTime regdate;
};

#endif // EUSER_H
