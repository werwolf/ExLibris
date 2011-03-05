#ifndef EUSER_H
#define EUSER_H

#include <QObject>
#include <QString>

class EUser : public QObject
{
    Q_OBJECT

public:
//    EUser();
    explicit EUser(long user_id);
    explicit EUser(QString login);
//    EUser(QString login,
//       QString password,
//       QString lasname,
//       QString name,
//       QString address = "",
//       QString phone = "",
//       QString email = "",
//       QString type = "'CLIENT'");

//    void newUser();

//signals:
//    void addUser(QString);

private:
    QString login;
    QString password;
    QString lasname;
    QString name;
    QString address;
    QString phone;
    QString email;
    QString type;
    QString regdate;
};

#endif // EUSER_H
