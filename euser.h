#ifndef EUSER_H
#define EUSER_H

#include <QtCore>

class EUser : public QObject
{
    Q_OBJECT

public:
    enum UserType {CLIENT, AUTHOR, SUPPLIER, OPERATOR, CEO, ADMIN};

    explicit EUser(long user_id);
    explicit EUser(QString _login);
    explicit EUser( const EUser& );
    EUser& operator=( const EUser& );
    virtual  ~EUser();

    long getID(void) const { return id; };
    QString getLogin (void) const { return login; };
    QString getLasname (void) const { return lasname; };
    QString getName (void) const { return name; };
    QString getAddress (void) const { return address; };
    QString getPhone (void) const { return phone; };
    QString getEmail (void) const { return email; };
    UserType getType(void ) const { return type; };

//signals:
//    void selectUser(QString);

public slots:
    void setUserInfo(QList<QStringList>);

//protected:
//    EUser( const EUser& );
//    EUser& operator=( const EUser& );

private:
    long    id;
    QString login;
//    QString password;
    QString lasname;
    QString name;
    QString address;
    QString phone;
    QString email;
    UserType  type;
//    QDateTime regdate;
};

#endif // EUSER_H
