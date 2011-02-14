#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>

namespace Ui {
    class LoginForm;
}

enum UserT {AUTHOR, CLIENT, SUPPLIER};

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QDialog *parent = 0);
    ~LoginForm();

private:
    Ui::LoginForm *ui;
    UserT usertype;

private slots:
    void on_toolBox_currentChanged(int index);
    void on_typeCBox_currentIndexChanged(int index);
};

#endif // LOGINFORM_H
