#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include <QtSql>

namespace Ui {
    class LoginForm;
}

enum UserT {CLIENT, AUTHOR, SUPPLIER};

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QDialog *parent = 0);
    LoginForm(QSqlDatabase *_db, QDialog *parent = 0);
    ~LoginForm();

private:
    Ui::LoginForm *ui;
    UserT usertype;
    QSqlDatabase *db;
    QSqlQuery *query;
    QValidator *phone_validator;

    // styles
    QString imp_field;
    QString non_imp_field;
    QString error_imp_field;
    QString error_non_imp_field;

    // private methods
    void loadStyleSheets(void);
    int checkUser(const QString login);
    int checkUser(const QString login, const QString pwd);

private slots:
    int   on_registrationButton_clicked();
    int   on_enterButton_clicked();
    void on_toolBox_currentChanged(int index);
    void on_typeCBox_currentIndexChanged(int index);

signals:
    int  loginning( int );
};

#endif // LOGINFORM_H
