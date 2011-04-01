#ifndef EADMIN_H
#define EADMIN_H

#include <QTabWidget>
#include "edbconnection.h"
#include "euser.h"

namespace Ui {
    class EAdmin;
}

class EAdmin : public QTabWidget, public EUser
{
    Q_OBJECT

public:
    explicit EAdmin(EUser& user, QWidget *parent = 0);
    ~EAdmin();

private slots:
    void on_execute_btn_clicked();
    void on_backup_btn_clicked();
    void on_restore_btn_clicked();
    void showError(QString);

private:
    Ui::EAdmin *ui;

    EDBconnection* db;

    QProcess *process;
};

#endif // EADMIN_H
