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
//    ~EUser();

signals:
    void selectUser(QString);

public slots:
    void setUserInfo(QList<QStringList>);

protected:
    EUser( const EUser& );
    EUser& operator=( const EUser& );

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
