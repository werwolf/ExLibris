#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include <QtSql>

namespace Ui {
    class LoginForm;
}

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QDialog *parent = 0);
    ~LoginForm();
    enum UserT {CLIENT, AUTHOR, SUPPLIER};

private:
    Ui::LoginForm *ui;
    UserT usertype;
    long user_id;
    QValidator *phone_validator;

    // styles
    QString imp_field;
    QString non_imp_field;
    QString error_imp_field;
    QString error_non_imp_field;

    // private methods
    void loadStyleSheets(void);

private slots:
    int  on_registrationButton_clicked();
    int  on_enterButton_clicked();
    void on_toolBox_currentChanged(int index);
    void on_typeCBox_currentIndexChanged(int index);

    void setUserId(long);

signals:
    void loginning( long );
    void checkUser(const QString login, const QString pwd = "ANY");
    void newAuthor(QString, QString, QString, QString, QDate, QString, QString, QString, QString);
    void newClient(QString, QString, QString, QString, QString, QString, QString, QString);
    void newSupplier(QString, QString, QString, QString, unsigned int, QString, QString, QString, QString);
};

#endif // LOGINFORM_H
