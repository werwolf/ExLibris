#include "eadmin.h"
#include "ui_eadmin.h"
#include <QMessageBox>
#include <QFileDialog>

EAdmin::EAdmin(EUser& user, QWidget *parent) :
    QTabWidget(parent),
    EUser(user),
    ui(new Ui::EAdmin)
{
    db = EDBconnection::getInstance();
    ui->setupUi(this);
    qDebug("EAdmin constructor");
//    connect(db, SIGNAL(returnLastError(QString)), ui->result_te, SLOT(insertHtml(QString)));
    connect(db, SIGNAL(returnLastError(QString)), this, SLOT(showError(QString)));

//    ui->result_te->insertHtml("<style type=”text/css”>p { line-height: 100px;}</style>");

    process = new QProcess;
}

EAdmin::~EAdmin()
{
    delete ui;
}

void EAdmin::on_execute_btn_clicked()
{
    if (ui->query_tx->document()->isEmpty()) return;

    QString query = ui->query_tx->toPlainText().trimmed();
    // show query
    qDebug()<<"query :"<<query;
    ui->result_te->insertHtml("<p style=\"color: blue;\">"+query+" : </p>");

    QString query_type = query.section(" ", 0, 0);
    if (QString::compare(query_type, QString("SELECT"), Qt::CaseInsensitive) == 0) {
        qDebug()<<"query_type :"<<query_type;
        QList<QStringList> List = db->get(query);

        // :TODO добавить вывод названия колонок
        if (!List.isEmpty()) {

            if (List.length() == 1 && List[0].length() == 1) {
                ui->result_te->insertHtml("<p>"+List[0].at(0)+"</p>");
            } else {
                QString html_table = "<table width=\"100%\" border=\"1\" cellspacing=\"0\" cellpadding=\"4\">";
                for (int i = 0; i < List.length(); ++i) {
                    QString row;
                    for (int j = 0; j < List[0].length(); ++j) {
                        row += "<td>" + List[i].at(j) + "</td>";
                    }
                    html_table += "<tr>"+row+"</tr>";
                }
                html_table += "</table><br/><br/><br/>";
                ui->result_te->insertHtml(html_table);
            }
        }

    } else if (QString::compare(query_type, QString("INSERT"), Qt::CaseInsensitive) == 0
               || QString::compare(query_type, QString("REPLACE"), Qt::CaseInsensitive) == 0) {

        // show last insert id
        if (int last_id = db->insert(query) != -1)
            ui->result_te->insertHtml(QString("<p style=\"color: green;\">Last insert ID = %1 </p><br/><br/>").arg(last_id));


    } else if (QString::compare(query_type, QString("UPDATE"), Qt::CaseInsensitive) == 0) {

    } else {
        // other queries {DELETE, i.e.}
        QString query = ui->query_tx->toPlainText().trimmed();
        bool error = db->query(query);
        if (error) ui->result_te->insertHtml("<p style=\"color: red;\">Error : EDBconnection::query(QString) return false;</p><br/><br/>");
    }
}

void EAdmin::on_backup_btn_clicked()
{
    // execute "mysqldump exlibris --user=root --password=********* > ./sql_log2.sql"
    //    QString param;
    //    param += " mysqldump ";
    //    param += db->get_dbName();
    //    param += " --host=" + db->get_dbHost();
    //    param += " --user=" + db->get_dbUser();
    //    param += " --password=" + db->get_dbPass();
    //    param += " --add-drop-table";
    //    param += " --complete-insert";
    //    param += " --triggers ";

    QString fileName = QFileDialog::getSaveFileName(this, QObject::trUtf8("Save Backup"), "", QObject::trUtf8("Save Backup (*.sql)"));
    qDebug()<<"filename :"<<fileName;
    if (fileName.isEmpty()) return;

    QStringList param;
    param << db->get_dbName();
    param << db->get_dbHost();
    param << db->get_dbUser();
    param << db->get_dbPass();
    param << fileName;

    qDebug()<<param;

    process->execute("./dump_db", param);

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // show ERROR message
        QMessageBox::warning(0, "Worning", "Ошибка.\nНельзя открыть файл.");
        return;
    }

    QByteArray db_data = file.readAll();
    ui->info_te->append(db_data);
}

void EAdmin::on_restore_btn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, trUtf8("Restore from Backup"), "", trUtf8("Load Backup (*.sql)"));
    qDebug()<<"filename :"<<fileName;

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // show ERROR message
        QMessageBox::warning(0, "Worning", trUtf8("Ошибка.\nНельзя открыть файл."));
        return;
    }

    QByteArray db_data = file.readAll();
    ui->info_te->setPlainText(db_data);

    QStringList param;
    param << db->get_dbName();
    param << db->get_dbHost();
    param << db->get_dbUser();
    param << db->get_dbPass();
    param << fileName;

    qDebug()<<param;

    process->execute("./restore_db", param);
}

void EAdmin::showError(QString error)
{
    ui->result_te->insertHtml("<p style=\"color: red; line-height: 10px;\">"+error+"</p><br/><br/>");
}
