#include <QtGui/QApplication>
//#include <QErrorMessage>
#include <QMessageBox>
#include <QSettings>
#include <QtSql/QtSql>
#include "mainwindow.h"
#include "loginform.h"

bool createConnection(QString host, QString database, QString user, QString password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", database);
    db.setHostName(host);
    db.setDatabaseName(database);
    db.setUserName(user);
    db.setPassword(password);
    if (!db.open()) {
//        QErrorMessage errorMessage;
//        errorMessage.showMessage(db.lastError().text());
//        errorMessage.exec();
        QMessageBox::warning(0, "ExLibris", db.lastError().text(), QMessageBox::Ok);
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Read DB settings from config.ini
    // !!! config file must be in the same directory as the executable program
    QSettings settings("config.ini", QSettings::IniFormat);
    QString strKey("DB.config/");
    QString strHost = settings.value( strKey + "host", "localhost").toString();
    QString strDatabase = settings.value( strKey + "database", "exlibris").toString();
    QString strUser = settings.value( strKey + "user").toString();
    QString strPassword = settings.value( strKey + "password").toString();

    // Connecting to DB
    if (!createConnection(strHost, strDatabase, strUser, strPassword))
        return 1;

    LoginForm login;
    login.show();

//    MainWindow w;
//    w.show();

    return a.exec();
}
