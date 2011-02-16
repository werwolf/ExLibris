#include <QtGui/QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QtSql>
#include "mainwindow.h"
#include "loginform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Read DB settings from config.ini
    // !!! config file must be in the same directory as the executable program
    QSettings settings("config.ini", QSettings::IniFormat);
    QString strKey("DB.config/");
    QString strHost = settings.value( strKey + "host", "localhost").toString();
    QString strDatabase = settings.value( strKey + "database", "exlibris").toString();
    strKey = "DB.config.user/";
    // Read user's login and password
    QString strUser = settings.value( strKey + "login","user").toString();
    QString strPassword = settings.value( strKey + "password","user_pwd").toString();

    // Connecting to DB    
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", strDatabase);
    db.setHostName(strHost);
    db.setDatabaseName(strDatabase);
    db.setUserName(strUser);
    db.setPassword(strPassword);
    if (!db.open()) {
        QMessageBox::warning(0, "ExLibris", db.lastError().text(), QMessageBox::Ok);
        return 1;
    }

    // Show login form
    LoginForm login(&db);
    login.show();

    MainWindow w(&db);
    QObject::connect(&login, SIGNAL(loginning(int)), &w, SLOT(startP(int)));

    return a.exec();
}
