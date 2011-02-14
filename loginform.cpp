#include "loginform.h"
#include "ui_loginform.h"

#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>

LoginForm::LoginForm(QSqlDatabase *_db, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm),
    db(_db),
    query(*_db)
{
    ui->setupUi(this);
    usertype = AUTHOR;
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
    switch(index) {
    case 0: usertype = AUTHOR;   resize(220,260); break;
    case 1: usertype = CLIENT;     resize(220,360); break;
    case 2: usertype = SUPPLIER; resize(220,370); break;
    default: break;
    }

    bool name_grp = false;
    bool info_grp = false;
    bool dist_grp = false;
    bool contact_grp = false;

    if (usertype == AUTHOR) {
        info_grp = true;
    }

    if (usertype == SUPPLIER) {
        dist_grp = true;
    }

    if (usertype == CLIENT || usertype == SUPPLIER) {
        name_grp = true;
        contact_grp = true;
    }

    // ##########################
    ui->nameEdit->setVisible(name_grp);
    ui->nameLabel->setVisible(name_grp);

    ui->lastnameEdit->setVisible(name_grp);
    ui->lastnameLabel->setVisible(name_grp);


    ui->sexCBox->setVisible(info_grp);
    ui->sexLabel->setVisible(info_grp);

    ui->birthdayDEdit->setVisible(info_grp);
    ui->birthdayLable->setVisible(info_grp);


    ui->distanceEdit->setVisible(dist_grp);
    ui->distanceLable->setVisible(dist_grp);


    ui->addressEdit->setVisible(contact_grp);
    ui->addressLable->setVisible(contact_grp);

    ui->phoneEdit->setVisible(contact_grp);
    ui->phoneLabel->setVisible(contact_grp);

    ui->emailEdit->setVisible(contact_grp);
    ui->emailLabel->setVisible(contact_grp);
}

void LoginForm::on_toolBox_currentChanged(int index)
{
    switch(index) {
    case 0:
        resize(220,180);
        break;
    case 1:
        resize(220,260);
        on_typeCBox_currentIndexChanged(usertype);
        break;
    }
}

void LoginForm::on_enterButton_clicked()
{
    QString login = ui->loginEdit->text();
    QString pwd  = ui->pwdEdit->text();

    // calculate password md5 hash
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(pwd.toUtf8());
    pwd = md5.result().toHex().constData();

    qDebug("login: %s\npassword: %s",login.toAscii().data(), pwd.toLocal8Bit().data());

    // find user
    QString query_str = QString("SELECT id FROM user WHERE login='%1' AND password='%2'").arg(login).arg(pwd);
    query.exec(query_str);

    // if error
    if (!query.isActive())
        QMessageBox::warning(this, tr("Database Error"), query.lastError().text());

    if (!query.next()) {
        qDebug("bad login or password");
        ui->loginEdit->setStyleSheet("* {background: rgb(240,0,0,80)}");
        ui->pwdEdit->setStyleSheet("* {background: rgb(240,0,0,80)}");
    } else {
        quint16 user_id     = query.value(0).toInt();
//        QString uset_type = query.value(1).toString();
        qDebug("login correct");
        // emit signal
        emit loginning(user_id);
        close();
    }
}
