#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include "esupplier.h"
#include "eclient.h"
#include "eoperator.h"
#include "eadmin.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    EOperator *e_operator;
    ESupplier *supplier;
    EAdmin *admin;
    EClient *client;

public slots:
    void startP(long user_id);
    void closeEvent(QCloseEvent *event);

signals:
    void exitWorkplace(void);
};

#endif // MAINWINDOW_H
