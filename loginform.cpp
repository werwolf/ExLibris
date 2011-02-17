#include "loginform.h"
#include "ui_loginform.h"

#include <QDebug>
#include <QMessageBox>

LoginForm::LoginForm(QSqlDatabase *_db, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm),
    db(_db),
    query(*_db)
{
    ui->setupUi(this);
    usertype = CLIENT;
    ui->loginEdit->setFocus();
}

LoginForm::LoginForm(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    usertype = AUTHOR;
    ui->loginEdit->setFocus();
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_typeCBox_currentIndexChanged(int index)
{
    bool info_grp = false;                // AUTHOR   : sex & birthday form
    bool dist_grp = false;                // SUPPLIER : distance form
    bool comp_grp = false;                // CLIENT || SUPPLIER : company name form

    switch(index) {    
    case 0:
        usertype = CLIENT;
        resize(220,330);
        comp_grp = true;
        break;
    case 1:
        usertype = AUTHOR;
        resize(220,370);
        info_grp = true;
        break;
    case 2:
        usertype = SUPPLIER;
        resize(220,360);
        dist_grp = comp_grp = true;
        break;
    default: break;
    }

    ui->sexCBox->setVisible(info_grp);
    ui->sexLabel->setVisible(info_grp);

    ui->birthdayDEdit->setVisible(info_grp);
    ui->birthdayLable->setVisible(info_grp);

    ui->distanceEdit->setVisible(dist_grp);
    ui->distanceLable->setVisible(dist_grp);

    ui->companyNameEdit->setVisible(comp_grp);
    ui->companyNameLabel->setVisible(comp_grp);
}

void LoginForm::on_toolBox_currentChanged(int index)
{
    switch(index) {
    case 0:
        resize(220,180);
        break;
    case 1:
        on_typeCBox_currentIndexChanged(usertype);
        break;
    }
}

int LoginForm::on_enterButton_clicked()
{
    qDebug()<<"authorization:";

    // highlighting empty fields on enter
    ui->loginEdit->setStyleSheet("* {background: rgb(255,255,255)}");
    ui->pwdEdit->setStyleSheet("* {background: rgb(255,255,255)}");

    if (ui->loginEdit->text().isEmpty()) {
        qDebug("login is empty\n");
        ui->loginEdit->setStyleSheet("* {background: rgb(240,0,0,80)}");
        ui->loginEdit->setFocus();
        return -1;
    }

    if (ui->pwdEdit->text().isEmpty()) {
        qDebug("password is empty\n");
        ui->pwdEdit->setStyleSheet("* {background: rgb(240,0,0,80)}");
        ui->pwdEdit->setFocus();
        return -1;
    }

    QString login = ui->loginEdit->text();
    QString pwd  = ui->pwdEdit->text();

    if (checkUser(login, pwd) == -1) {
        qDebug("bad login or password\n");
        ui->loginEdit->setStyleSheet("* {background: rgb(240,0,0,80)}");
        ui->pwdEdit->setStyleSheet("* {background: rgb(240,0,0,80)}");
        return -1;
    } else {
        qDebug("login correct\n");
        quint16 user_id = query.value(0).toInt();
        // emit signal.
        emit loginning(user_id);
        close();
    }
    return 0;
}

void LoginForm::on_registrationButton_clicked()
{
    qDebug()<<"registration:";
}

int LoginForm::checkUser(QString login, QString pwd)
{
    qDebug("login: %s\npassword: %s",qPrintable(login), qPrintable(pwd));

    // find user
    QString query_str = QString("SELECT id FROM users WHERE login='%1' AND password=MD5('%2')").arg(login).arg(pwd);
    query.exec(query_str);

    // if error
    if (!query.isActive())
        QMessageBox::warning(this, tr("Database Error"), query.lastError().text());

    if (!query.next()) {
        // can not found user with this pair of login and password.
        return -1;
    } else {
        // user is found. return user_id.
        quint16 user_id = query.value(0).toInt();
        return user_id;
    }
}
