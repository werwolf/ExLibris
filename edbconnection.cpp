#include "edbconnection.h"
#include <QSettings>
#include <QSqlQuery>
#include <QSqlDriver>
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

//    QSqlDriver *driver = QSqlDatabase::database().driver();
//    if (driver->hasFeature(QSqlDriver::Transactions)) qDebug("transaction is ok."); else qDebug("transaction is bad.");
    query("SET NAMES 'utf8'");
    query("SET character_set_results = 'utf8'");
//    query("SET AUTOCOMMIT = 0");
}

EDBconnection::~EDBconnection()
{
    db.close();
    qDebug("\nclosed connection to database");
}

//void EDBconnection::destroyInstance()
//{
//    if (pinstance) delete pinstance;
//}

QString EDBconnection::escape(const QString q) const
{
    QString res = q;
    return res.replace("\\", "\\\\")
              .replace("\"", "\\\"")
              .replace("'", "\\'").replace("\n", "\\n")
              .replace("\r", "\\r")/*.replace("\x00", "\\0")*/
              .replace("\b", "\\b").replace("\t", "\\t")
              .replace("\x32", "\\Z")
              .replace("_", "\\_").replace(("%"), "\\%");
}

bool EDBconnection::query(const QString query) const
{
    qDebug()<<"[query]\t:"<<query;
    QSqlQuery sqlQuery(query, db);

    if (!sqlQuery.isActive()) {
        qDebug()<<"ERROR\t:"<<sqlQuery.lastError().text();
        return false;
    }
    return true;
}

QList<QStringList> EDBconnection::get(const QString query) const
{
    qDebug()<<"[get]\t:"<<query;
    QList<QStringList> List;
    QSqlQuery sqlQuery(query,db);
    QSqlRecord rec = sqlQuery.record();
    QStringList row;

    while( sqlQuery.next() ){
        row.clear();
        for( int i=0; i < rec.count(); ++i ) {
            row << sqlQuery.value(i).toString();
        }
        List << row;
    }

    if (!sqlQuery.isActive()) {
        qDebug()<<"ERROR\t:"<<sqlQuery.lastError().text();
    } else {
        qDebug()<<"return\t:"<<List;
//        emit returnSelQuery(List);
    }
    return List;
}

int EDBconnection::insert(const QString query) const
{
    qDebug()<<"[insert]\t:"<<query;
    QSqlQuery sqlQuery(query, db);

    if (!sqlQuery.isActive()) {
        qDebug()<<"error :"<<sqlQuery.lastError().text();
        return -1;
    }
    return QVariant(sqlQuery.lastInsertId()).toInt();
}

void EDBconnection::checkUser(QString login, QString pwd)
{
    login.remove(QRegExp("['\"]"));
    QString query;

    // find user
    if (pwd != "ANY") {
        pwd.remove(QRegExp("['\"]"));
        qDebug("> login: %s\n> password: %s",qPrintable(login), qPrintable(pwd));
        // need smth like this: "SELECT id FROM users WHERE MD5(CONCAT(login, password)) = '"+ md5(login.pwd) +"'"
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

int EDBconnection::newUser(QString login,
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

    QString sqlQuery = QString("INSERT INTO users " \
                               "(login, password, lastname, name, address, phone, email, type, reg_date) " \
                               "VALUES ('%1', MD5('%2'), '%3', '%4', '%5', '%6', '%7', '%8', '%9')")
                       .arg(login).arg(password).arg(lastname).arg(name).arg(address)
                       .arg(phone).arg(email).arg(type).arg(regdate.toString("yyyy-MM-dd HH-m-s"));
    return insert(sqlQuery);
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
    // :TODO make transaction
//    query.exec("START TRANSACTION;"); if (!query.isActive()) qDebug()<<"error: "<<query.lastError().text();
//    QSqlDatabase::database().transaction();

    int user_id = newUser(login, password, lastname, name, address, phone, email,"AUTHOR");
    insert(QString("INSERT INTO authors (user_id, dob, sex) VALUES ('%1', '%2', '%3')")
           .arg(user_id).arg(birth_date.toString("yyyy-MM-dd")).arg(sex));

//    QSqlDatabase::database().commit();
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
    int user_id = newUser(login, password, lastname, name, address, phone, email,"CLIENT");
    insert(QString("INSERT INTO clients (user_id, company_name) VALUES ('%1', '%2')")
           .arg(user_id).arg(company_name));
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
    int user_id = newUser(login, password, lastname, name, address, phone, email,"SUPPLIER");
    insert(QString("INSERT INTO suppliers (user_id, distance, company_name) VALUES ('%1', '%2', '%3')")
           .arg(user_id).arg(dist).arg(company_name));
}
