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
    supplier = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete supplier;
}

void MainWindow::startP(long user_id)
{
    EUser user(user_id);
    qDebug()<<">> User"<<user.getName()<<"has been connected.";

    if (user.getType() == EUser::CLIENT) {
        EClient client(user);
        qDebug()<<">> CLIENT"<<client.getName()<<"has been connected.";
    } else if (user.getType() == EUser::AUTHOR) {

        qDebug()<<">> AUTHOR"/*<<client.getName()*/<<"has been connected.";
    } else if (user.getType() == EUser::SUPPLIER) {
        supplier = new ESupplier(user);
        this->setCentralWidget(supplier->window());
        this->setWindowTitle(QString("Supplier : \"%1\"").arg(supplier->getName()));
        this->resize(680, 270);
        qDebug()<<">> SUPPLIER"/*<<client.getName()*/<<"has been connected.";
    } else if (user.getType() == EUser::CEO) {

        qDebug()<<">> CEO"/*<<client.getName()*/<<"has been connected.";
    } else if (user.getType() == EUser::OPERATOR) {

        qDebug()<<">> OPERATOR"/*<<client.getName()*/<<"has been connected.";
    } else if (EUser::ADMIN) {

        qDebug()<<">> ADMIN"/*<<client.getName()*/<<"has been connected.";
    }

    show();
}
