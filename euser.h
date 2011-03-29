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

    long getUserID(void) const { return user_id; }
    QString getLogin (void) const { return login; }
    QString getlastname (void) const { return lastname; }
    QString getName (void) const { return name; }
    QString getAddress (void) const { return address; }
    QString getPhone (void) const { return phone; }
    QString getEmail (void) const { return email; }
    UserType getType(void ) const { return type; }

    void setUserID(const long _user_id) { user_id = _user_id; }
    void setLogin (const QString _login) { login = _login; }
    void setlastname (const QString _lastname) { lastname = _lastname; }
    void setName (const QString _name) { name = _name; }
    void setAddress (const QString _address) { address = _address; }
    void setPhone (const QString _phone) { phone = _phone; }
    void setEmail (const QString _email) { email = _email; }
    void setType(const UserType _type) { type = _type; }

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
