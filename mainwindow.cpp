#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "euser.h"
#include "eclient.h"
//#include "esupplier.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    e_operator = 0;
    supplier = 0;
    client = 0;
    admin = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete e_operator;
    delete supplier;
    delete client;
    delete admin;
}

void MainWindow::startP(long user_id)
{
    EUser user(user_id);
    qDebug()<<">> User"<<user.getName()<<"has been connected.";

    if (user.getType() == EUser::CLIENT) {
        client = new EClient(user);
        this->setCentralWidget(client->window());
        this->setWindowTitle(QString("Client : \"%1 %2\"").arg(client->getName()).arg(client->getlastname()));
        this->resize(680, 270);
        qDebug()<<">> CLIENT has been connected.";

    } else if (user.getType() == EUser::AUTHOR) {

        qDebug()<<">> AUTHOR has been connected.";

    } else if (user.getType() == EUser::SUPPLIER) {
        supplier = new ESupplier(user);
        this->setCentralWidget(supplier->window());
        this->setWindowTitle(QString("Supplier : \"%1 %2\"").arg(supplier->getName()).arg(supplier->getlastname()));
        this->resize(680, 270);
        qDebug()<<">> SUPPLIER has been connected.";

    } else if (user.getType() == EUser::CEO) {

        qDebug()<<">> CEO has been connected.";

    } else if (user.getType() == EUser::OPERATOR) {
        e_operator = new EOperator(user);
        this->setCentralWidget(e_operator->window());
        this->setWindowTitle(QString("Operator : \"%1 %2\"").arg(e_operator->getName()).arg(e_operator->getlastname()));
        this->resize(680, 270);
        qDebug()<<">> OPERATOR has been connected.";

    } else if (EUser::ADMIN) {
        admin = new EAdmin(user);
        this->setCentralWidget(admin->window());
        this->setWindowTitle(QString("Client : \"%1 %2\"").arg(admin->getName()).arg(admin->getlastname()));
//        this->resize(680, 270);
        qDebug()<<">> ADMIN has been connected.";
    }

    show();
}
