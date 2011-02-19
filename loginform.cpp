#include "loginform.h"
#include "ui_loginform.h"

#include <QDebug>
#include <QMessageBox>

LoginForm::LoginForm(QSqlDatabase *_db, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm),
    db(_db),
    query(new QSqlQuery(*_db))
{
    ui->setupUi(this);
    loadStyleSheets();
    usertype = CLIENT;
    ui->loginEdit->setFocus();

    // i.e. "+46(0)31-445566"
    // QRegExp regexPhone("^\\+\\d{1,2}\\([089]\\)\\d{2,5}\\-\\d+$");

    // my: "+38 (ddd) ddd dd dd"
    QRegExp regexPhone("^\\+\\d{1,2}\\s\\(\\d{3}\\)\\s\\d{3}\\s\\d{2}\\s\\d{2}$");
    QValidator *phone_validator = new QRegExpValidator(regexPhone, 0);
    ui->phoneEdit->setValidator(phone_validator);

    ui->distanceEdit->setValidator(new QIntValidator(this));
}

LoginForm::LoginForm(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    loadStyleSheets();
    usertype = AUTHOR;
    ui->loginEdit->setFocus();
}

LoginForm::~LoginForm()
{
    delete ui;
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
    bool info_grp = false;                // AUTHOR   : sex & birthday form
    bool dist_grp = false;                // SUPPLIER : distance form
    bool comp_grp = false;             // CLIENT || SUPPLIER : company name form

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
        ui->loginEdit->setFocus();
        return -1;
    } else {
        qDebug("login correct\n");
        quint16 user_id = query->value(0).toInt();
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
        QRegExp regexEmail("^[^_\\.][a-z0-9_]+[\\.]?[a-z0-9_]+[^_\\.]@{1}[a-z0-9]+[\\.]{1}(([a-z]{2,4})|([a-z]{2,4}[\\.]{1}[a-z]{2,3}))$" );
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
    if (checkUser(ui->regLoginEdit->text()) > 0) {
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
        QString company = ui->companyNameEdit->text();          // CLIENT && SUPPLIER
        unsigned int dist = ui->distanceEdit->text().toInt();         // SUPPLIER only
        QDate birthday = ui->birthdayDEdit->date();                    // AUTHOR
        QChar sex;                                                                         // AUTHOR
        switch (ui->sexCBox->currentIndex()) {
        case 1: sex = 'M'; break;
        case 2: sex = 'W'; break;
        }

        switch (usertype) {
        case CLIENT:
            break;
        case AUTHOR:
            break;
        case SUPPLIER:
            break;
        default: break;
        }

//        QSqlDatabase::database().transaction();
//        query.prepare("INSERT INTO users (id, login, password, lasname, name, address, phone, email, type, regdate)"
//        "VALUES (:id, :login, :password, :lasname, :name, :address, :phone, :email, :type, :regdate)");
//        query.bindValue(":id", "NULL");
//        query.bindValue(":login", );
//        query.bindValue(":password", );
//        query.bindValue(":lasname", );
//        query.bindValue(":name", );
//        query.bindValue(":address", );
//        query.bindValue(":phone", );
//        query.bindValue(":email", );
//        query.bindValue(":type", );
//        query.bindValue(":regdate", );
//        QSqlDatabase::database().commit();
    }

    return 0;
}

int LoginForm::checkUser(const QString login, const QString pwd)
{
    qDebug("login: %s\npassword: %s",qPrintable(login), qPrintable(pwd));

    // find user
    QString query_str = QString("SELECT id FROM users WHERE login='%1' AND password=MD5('%2')").arg(login).arg(pwd);
    query->exec(query_str);

    // if error
    if (!query->isActive())
        QMessageBox::warning(this, tr("Database Error"), query->lastError().text());

    if (!query->next()) {
        // can not found user with this pair of login and password.
        return -1;
    } else {
        // user is found. return user_id.
        quint16 user_id = query->value(0).toInt();
        return user_id;
    }
}

int LoginForm::checkUser(const QString login)
{
    qDebug("login: %s",qPrintable(login));

    // find user
    QString query_str = QString("SELECT id FROM users WHERE login='%1'").arg(login);
    query->exec(query_str);

    // if error
    if (!query->isActive())
        QMessageBox::warning(this, tr("Database Error"), query->lastError().text());

    if (!query->next()) {
        // can not found user with this login
        return -1;
    } else {
        // user is found. return user_id.
        quint16 user_id = query->value(0).toInt();
        return user_id;
    }
}
