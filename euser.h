#ifndef EUSER_H
#define EUSER_H

#include "QString"

class EUser
{
public:
    EUser();
    static void newUser(QString id,
                   QString login,
                   QString password,
                   QString lasname,
                   QString name,
                   QString address,
                   QString phone,
                   QString email,
                   QString type,
                   QString regdate);
};

#endif // EUSER_H
