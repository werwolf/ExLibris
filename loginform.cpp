#include "loginform.h"
#include "ui_loginform.h"

#include <QDebug>
#include <QMessageBox>
#include "edbconnection.h"

LoginForm::LoginForm(QDialog *parent) : QDialog(parent), ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    loadStyleSheets();
    usertype = CLIENT;
    user_id = -1;
    ui->loginEdit->setFocus();

    // i.e. "+46(0)31-445566"
    QRegExp regexPhone("^(?!:\\A|\\s)(?!(\\d{1,6}\\s+\\D)|((\\d{1,2}\\s+){2,2}))(((\\+\\d{1,3})|(\\(\\+\\d{1,3}\\)))\\s*)?((\\d{1,6})|(\\(\\d{1,6}\\)))\\/?(([ -.]?)\\d{1,5}){1,5}((\\s*(#|x|(ext))\\.?\\s*)\\d{1,5})?(?!:(\\Z|\\w|\\b\\s))$");
    QValidator *phone_validator = new QRegExpValidator(regexPhone, 0);
    ui->phoneEdit->setValidator(phone_validator);

    // validate username
    QRegExp regexVal("^[\\w_]{6,18}$");
    QValidator *username_validator = new QRegExpValidator(regexVal, 0);
    ui->regLoginEdit->setValidator(username_validator);

    // validate username and password
    QValidator *password_validator = new QRegExpValidator(regexVal, 0);
    ui->regPwdEdit->setValidator(password_validator);

    ui->lastnameEdit->setValidator(new QRegExpValidator(QRegExp("^[a-zA-Z\\s-']{3,45}$"), 0));
    ui->nameEdit->setValidator(new QRegExpValidator(QRegExp("^[a-zA-Z\\s-']{3,45}$"), 0));
    ui->distanceEdit->setValidator(new QIntValidator(this));

    QObject::connect(this, SIGNAL(checkUser(QString, QString)), EDBconnection::getInstance(), SLOT(checkUser(QString, QString)));
    QObject::connect(EDBconnection::getInstance(), SIGNAL(setUserId(long)), this, SLOT(setUserId(long)));

    QObject::connect(this,
                     SIGNAL(newAuthor(QString,QString,QString,QString,QDate,QString,QString,QString,QString)),
                     EDBconnection::getInstance(),
                     SLOT(newAuthor(QString,QString,QString,QString,QDate,QString,QString,QString,QString))
                     );
    QObject::connect(this,
                     SIGNAL(newClient(QString,QString,QString,QString,QString,QString,QString,QString)),
                     EDBconnection::getInstance(),
                     SLOT(newClient(QString,QString,QString,QString,QString,QString,QString,QString))
                     );
    QObject::connect(this,
                     SIGNAL(newSupplier(QString,QString,QString,QString,uint,QString,QString,QString,QString)),
                     EDBconnection::getInstance(),
                     SLOT(newSupplier(QString,QString,QString,QString,uint,QString,QString,QString,QString))
                     );
}

LoginForm::~LoginForm()
{
    delete ui;
//    delete username_validator;
//    delete password_validator;
//    delete phone_validator;
}

void LoginForm::loadStyleSheets(void)
{
    QFile file_imp_field(":/loginform/qss/imp_field.qss");
    file_imp_field.open(QFile::ReadOnly);
    imp_field = QLatin1String(file_imp_field.readAll());

    QFile file_non_imp_field(":/loginform/qss/non_imp_field.qss");
    file_non_imp_field.open(QFile::ReadOnly);
    non_imp_field = QLatin1String(file_non_imp_field.readAll());

    QFile file_error_imp_field(":/loginform/qss/error_imp_field.qss");
    file_error_imp_field.open(QFile::ReadOnly);
    error_imp_field = QLatin1String(file_error_imp_field.readAll());

    QFile file_error_non_imp_field(":/loginform/qss/error_non_imp_field.qss");
    file_error_non_imp_field.open(QFile::ReadOnly);
    error_non_imp_field = QLatin1String(file_error_non_imp_field.readAll());
}

void LoginForm::on_typeCBox_currentIndexChanged(int index)
{
    bool info_grp = false;             // AUTHOR   : sex & birthday form
    bool dist_grp = false;             // SUPPLIER : distance form
    bool comp_grp = false;             // CLIENT || SUPPLIER : company name form

    switch(index) {
    case 0:
        usertype = CLIENT;
        resize(220,335);
        comp_grp = true;
        break;
    case 1:
        usertype = AUTHOR;
        resize(220,375);
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
    case 0:                     // login tab
        this->setWindowTitle(QString::fromUtf8("Вход в систему"));
        resize(220,180);
        break;
    case 1:                     // registration tab
        this->setWindowTitle(QString::fromUtf8("Регистрация"));
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

    emit checkUser(login, pwd);

    if (user_id == -1) {
        qDebug("bad login or password\n");
        ui->loginEdit->setStyleSheet("* {background: rgb(240,0,0,80)}");
        ui->pwdEdit->setStyleSheet("* {background: rgb(240,0,0,80)}");
        ui->loginEdit->setFocus();
        return -1;
    } else {
        qDebug("login correct\n");
        // emit signal.
        emit loginning(user_id);
        close();
    }
    return 0;
}

int LoginForm::on_registrationButton_clicked()
{
    qDebug()<<"registration:";

    // highlighting empty fields on enter
    ui->regLoginEdit->setStyleSheet(imp_field);
    ui->regPwdEdit->setStyleSheet(imp_field);
    ui->lastnameEdit->setStyleSheet(imp_field);
    ui->nameEdit->setStyleSheet(imp_field);
    //
    ui->companyNameEdit->setStyleSheet(imp_field);
    ui->distanceEdit->setStyleSheet(imp_field);
    //
    ui->emailEdit->setStyleSheet(non_imp_field);

    if (ui->regLoginEdit->text().isEmpty()) {
        qDebug("login is empty\n");
        ui->regLoginEdit->setStyleSheet(error_imp_field);
        ui->regLoginEdit->setFocus();
        return -1;
    }

    if (ui->regPwdEdit->text().isEmpty()) {
        qDebug("password is empty\n");
        ui->regPwdEdit->setStyleSheet(error_imp_field);
        ui->regPwdEdit->setFocus();
        return -1;
    }

    if (ui->lastnameEdit->text().isEmpty()) {
        qDebug("lastname is empty\n");
        ui->lastnameEdit->setStyleSheet(error_imp_field);
        ui->lastnameEdit->setFocus();
        return -1;
    }

    if (ui->nameEdit->text().isEmpty()) {
        qDebug("name is empty\n");
        ui->nameEdit->setStyleSheet(error_imp_field);
        ui->nameEdit->setFocus();
        return -1;
    }

    //
    if (ui->distanceEdit->text().isEmpty() && usertype == SUPPLIER) {
        qDebug("distance is empty\n");
        ui->distanceEdit->setStyleSheet(error_imp_field);
        ui->distanceEdit->setFocus();
        return -1;
    }

    if (ui->companyNameEdit->text().isEmpty() && (usertype == CLIENT || usertype == SUPPLIER)) {
        qDebug("company name is empty\n");
        ui->companyNameEdit->setStyleSheet(error_imp_field);
        ui->companyNameEdit->setFocus();
        return -1;
    }

    //
    if (!ui->emailEdit->text().isEmpty()) {
        QRegExp regexEmail("^[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*"
                           "@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?$");
        QRegExpValidator validator(regexEmail, 0);

        int pos = 0;
        QString email = ui->emailEdit->text();
        if( validator.validate(email, pos) != QValidator::Acceptable ) {
            qDebug("error email format\n");
            ui->emailEdit->setStyleSheet(error_non_imp_field);
            ui->emailEdit->setFocus();
            return -1;
        }
    }

    //
    emit checkUser(ui->regLoginEdit->text());
    if (user_id > 0) {
        qDebug("user with this login is already exist\n");
        ui->regLoginEdit->setStyleSheet(error_imp_field);
        ui->regLoginEdit->setFocus();
        return -1;
    } else {
        qDebug("registration data is correc\n");

        QString login = ui->regLoginEdit->text();
        QString pwd = ui->regPwdEdit->text();
        QString lastname = ui->lastnameEdit->text();
        QString name = ui->nameEdit->text();
        QString address = ui->addressEdit->text();
        QString phone = ui->phoneEdit->text();
        QString email = ui->emailEdit->text();
        //
        QString company = ui->companyNameEdit->text();              // CLIENT && SUPPLIER
        unsigned int dist  = ui->distanceEdit->text().toInt();      // SUPPLIER only
        QDate birthday = ui->birthdayDEdit->date();                 // AUTHOR
        QString sex;                                                // AUTHOR
        switch (ui->sexCBox->currentIndex()) {
        case 0: sex = "M"; break;
        case 1: sex = "W"; break;
        }

        switch (usertype) {
        case CLIENT:
            emit newClient(login, pwd, lastname, name, company, address, phone, email);
            break;
        case AUTHOR:
            emit newAuthor(login, pwd, lastname, name, birthday, sex, address, phone, email);
            break;
        case SUPPLIER:
            emit newSupplier(login, pwd, lastname, name, dist, company, address, phone, email);
            break;
        default: break;
        }
    }

    return 0;
}

void LoginForm::setUserId(long user_id)
{
    this->user_id = user_id;
}
