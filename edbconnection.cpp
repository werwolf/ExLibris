#include "edbconnection.h"
#include <QSettings>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExp>
#include <QMessageBox>

EDBconnection* EDBconnection::pinstance = 0;  // initialize pointer

EDBconnection* EDBconnection::getInstance()
{
    if (!pinstance) {
        try {
            pinstance = new EDBconnection;
        }
        catch (QString error) {
            QMessageBox::warning(0, "ExLibris", error, QMessageBox::Ok);
            delete pinstance;
            throw (int) 3;
        }
        catch (...) {
            qWarning("DB connection error");
            delete pinstance;
            throw (int) 2;
        }
    }
    Q_ASSERT(pinstance != NULL);
    return pinstance;
}

EDBconnection::EDBconnection()
{
    // Read DB settings and user's login and password from config.ini
    // !!! config file must be in the same directory as the executable program
    QSettings settings("config.ini", QSettings::IniFormat);
    QString strKey("DB.config/");
    dbHost = settings.value(strKey + "host", "localhost").toString();
    dbName = settings.value(strKey + "database", "exlibris").toString();
    dbUser = settings.value(strKey + "login").toString();
    dbPass = settings.value(strKey + "password").toString();

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(dbHost);
    db.setDatabaseName(dbName);
    db.setUserName(dbUser);
    db.setPassword(dbPass);

    if (!db.open()) {
        qDebug("Error occured in connection to database");
        throw db.lastError().text();
    }

    qDebug("connected to database \n");
}

EDBconnection::~EDBconnection()
{
    db.close();
    qDebug("close DB");
}

void EDBconnection::executeSqlQuery(const QString query) const
{
    QSqlQuery sqlQuery(query, db);
}

void EDBconnection::checkUser(QString login, QString pwd)
{
    login.remove(QRegExp("['\"]"));
    QString query;

    // find user
    if (pwd != "ANY") {
        pwd.remove(QRegExp("['\"]"));
        qDebug("> login: %s\n> password: %s",qPrintable(login), qPrintable(pwd));
        query = "SELECT id FROM users WHERE login='"+login+"' AND password=MD5('"+pwd+"')";
    }  else {
        qDebug("> login: %s",qPrintable(login));
        query = "SELECT id FROM users WHERE login='"+login+"'";
    }

    QSqlQuery sqlQuery(query, db);

    // if error
    if (!sqlQuery.isActive())
        QMessageBox::warning(0, "Database Error", sqlQuery.lastError().text());

    if (sqlQuery.next()) {
        // user is found. return user_id.
        long user_id = sqlQuery.value(0).toInt();
        emit setUserId(user_id);
//        QMetaObject::invokeMethod(sender(), "setUserId", Qt::DirectConnection, Q_ARG(long, user_id));
    } else {
        // can not found user with this pair of login and password.
        emit setUserId(-1);
    }
}
