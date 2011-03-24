#include <QtGui/QApplication>
#include "mainwindow.h"
#include "loginform.h"
#include "edbconnection.h"

#include "esupplier.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EDBconnection* m_db;

    try {
        m_db = EDBconnection::getInstance();
    }
    catch (int error) {
        qDebug("error (main) when try connect to DB");
        return error;
    }
    catch (...) {
        qDebug("error (main) when try connect to DB");
        return 1;
    }

    LoginForm login;
    MainWindow w;

    QObject::connect(&login, SIGNAL(loginning(long)), &w, SLOT(startP(long)));

//    login.show();

    EUser user(19);
    ESupplier supplier(user);
    supplier.show();

    int r_code = a.exec();
    delete m_db;
    return r_code;
}
