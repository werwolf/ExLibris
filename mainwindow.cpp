#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QSqlDatabase *_db, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(_db),
    query(*_db)
{
    ui->setupUi(this);
}

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


void MainWindow::startP(int user_id)
{
    show();
}
