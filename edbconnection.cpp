#include "edbconnection.h"
#include <QSettings>
#include <QSqlQuery>
#include <QSqlRecord>
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

//void EDBconnection::destroyInstance()
//{
//    if (pinstance) delete pinstance;
//}

void EDBconnection::executeSqlQuery(const QString query) const
{
    QSqlQuery sqlQuery(query, db);
}

void EDBconnection::executeSelQuery(const QString query)
{
    QList<QStringList> List;
    QStringList fieldName;
    QSqlQuery sqlQuery(query,db);
    QSqlRecord rec = sqlQuery.record();
    for( int i=0; i<rec.count(); ++i )
        fieldName << rec.fieldName( i );

    foreach( QString str, fieldName ){
        QStringList tmpList;
        while( sqlQuery.next() ){
            tmpList << sqlQuery.value(rec.indexOf( str )).toString();
        }
        sqlQuery.seek(-1);  // since next at end - start at top again for next pass
        List << tmpList;
    }
    qDebug()<<List;
    emit setUserInfo(List);
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

void EDBconnection::newUser(QString login,
                            QString password,
                            QString lastname,
                            QString name,
                            QString address = "",
                            QString phone = "",
                            QString email = "",
                            QString type = "CLIENT")
{
    QDateTime regdate = QDateTime::currentDateTime();
    qDebug()<<regdate;

    QSqlQuery sqlQuery(db);
    sqlQuery.prepare("INSERT INTO users (id, login, password, lastname, name, address, phone, email, type, reg_date)"
        "VALUES (NULL, ?, MD5(?), ?, ?, ?, ?, ?, ?, ?)");
    sqlQuery.bindValue(0, login);
    sqlQuery.bindValue(1, password);
    sqlQuery.bindValue(2, lastname);
    sqlQuery.bindValue(3, name);
    sqlQuery.bindValue(4, address);
    sqlQuery.bindValue(5, phone);
    sqlQuery.bindValue(6, email);
    sqlQuery.bindValue(7, type);
    sqlQuery.bindValue(8, regdate);
    sqlQuery.exec();

    // if error
    if (!sqlQuery.isActive()) qDebug()<<sqlQuery.lastError().text();
}

void EDBconnection::newAuthor(QString login,
                            QString password,
                            QString lastname,
                            QString name,
                            QDate   birth_date,
                            QString sex,
                            QString address = "",
                            QString phone = "",
                            QString email = "")
{
    QSqlQuery query(db);

    // :TODO make transaction
//    query.exec("START TRANSACTION;"); if (!query.isActive()) qDebug()<<"error: "<<query.lastError().text();

    newUser(login, password, lastname, name, address, phone, email,"AUTHOR");

    query.prepare("INSERT INTO authors (id, user_id, dob, sex) VALUES (NULL, LAST_INSERT_ID(), ?, ?)");
    query.bindValue(0, birth_date);
    query.bindValue(1, sex);
    query.exec();
    // if error
    if (!query.isActive()) qDebug()<<"error: "<<query.lastError().text();

//    query.exec("COMMIT;"); if (!query.isActive()) qDebug()<<"error: "<<query.lastError().text();


}

void EDBconnection::newClient(QString login,
                            QString password,
                            QString lastname,
                            QString name,
                            QString company_name,
                            QString address = "",
                            QString phone = "",
                            QString email = "")
{
    QSqlQuery query(db);

    newUser(login, password, lastname, name, address, phone, email,"CLIENT");

    query.prepare("INSERT INTO clients (id, user_id, company_name) VALUES (NULL, LAST_INSERT_ID(), ?)");
    query.bindValue(0, company_name);
    query.exec();
    // if error
    if (!query.isActive()) qDebug()<<"error: "<<query.lastError().text();

}

void EDBconnection::newSupplier(QString login,
                            QString password,
                            QString lastname,
                            QString name,
                            unsigned int dist,
                            QString company_name,
                            QString address = "",
                            QString phone = "",
                            QString email = "")
{
    QSqlQuery query(db);

    newUser(login, password, lastname, name, address, phone, email,"SUPPLIER");

    query.prepare("INSERT INTO suppliers (id, user_id, distance, company_name) VALUES (NULL, LAST_INSERT_ID(), ?, ?)");
    query.bindValue(0, dist);
    query.bindValue(1, company_name);
    query.exec();
    // if error
    if (!query.isActive()) qDebug()<<"error: "<<query.lastError().text();
}
