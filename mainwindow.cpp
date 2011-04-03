#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "euser.h"
#include "eclient.h"
//#include "esupplier.h"
#include <QMessageBox>
#include <QCloseEvent>

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
    qDebug("MainWindow destructor");
    delete e_operator;
    delete supplier;
    delete client;
    delete admin;
    delete ui;
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
        this->resize(700, 280);
        qDebug()<<">> SUPPLIER has been connected.";

    } else if (user.getType() == EUser::CEO) {

        qDebug()<<">> CEO has been connected.";

    } else if (user.getType() == EUser::OPERATOR) {
        e_operator = new EOperator(user);
        this->setCentralWidget(e_operator->window());
        this->setWindowTitle(QString("Operator : \"%1 %2\"").arg(e_operator->getName()).arg(e_operator->getlastname()));
        this->resize(790, 270);
        qDebug()<<">> OPERATOR has been connected.";

    } else if (EUser::ADMIN) {
        admin = new EAdmin(user);
        this->setCentralWidget(admin->window());
        this->setWindowTitle(QString("Admin : \"%1 %2\"").arg(admin->getName()).arg(admin->getlastname()));
        this->resize(550, 400);
        qDebug()<<">> ADMIN has been connected.";
    }

    show();
}

//обработчик выхода
void MainWindow::closeEvent(QCloseEvent* event)
{
    //выдаём сообщение
    int result = QMessageBox::question(0, trUtf8("Предупреждение"),
                                       trUtf8("Вы действительно хотите выйти?"),
                                       QMessageBox::Yes, QMessageBox::No);

    //анализируем ответ
    if (result == QMessageBox::Yes){
        //выходим
        emit exitWorkplace();
        event->accept();
    }
    else{
        //остаёмся
        event->ignore();
    }
}
