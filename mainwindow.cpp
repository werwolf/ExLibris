#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "euser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startP(long user_id)
{
    EUser user(user_id);
    show();
}
