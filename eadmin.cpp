#include "eadmin.h"
#include "ui_eadmin.h"

EAdmin::EAdmin(EUser& user, QWidget *parent) :
    QTabWidget(parent),
    EUser(user),
    ui(new Ui::EAdmin)
{
    ui->setupUi(this);
    qDebug("EAdmin constructor");
    connect(EDBconnection::getInstance(), SIGNAL(returnLastError(QString)), ui->result_te, SLOT(append(QString)));
}

EAdmin::~EAdmin()
{
    delete ui;
}

void EAdmin::on_execute_btn_clicked()
{
    if (ui->query_tx->document()->isEmpty()) return;

    QString query = ui->query_tx->toPlainText().trimmed();
    qDebug()<<"query :"<<query;
    QString query_type = query.section(" ", 0, 0);
    if (QString::compare(query_type, QString("SELECT"), Qt::CaseInsensitive) == 0) {
        qDebug()<<"query_type :"<<query_type;
        QList<QStringList> List = EDBconnection::getInstance()->get(query);

        // :TODO добавить вывод названия колонок
        if (!List.isEmpty()) {
            // show query
            ui->result_te->insertHtml("<p color=\"blue\">"+query+"</p>");

            if (List.length() == 1 && List[0].length() == 1) {
                ui->result_te->append(List[0].at(0));
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
        if (int last_id = EDBconnection::getInstance()->insert(query) != -1)
            ui->result_te->insertHtml(QString("<p color=\"blue\">Last insert ID = %1 </p>").arg(last_id));


    } else if (QString::compare(query_type, QString("UPDATE"), Qt::CaseInsensitive) == 0) {

    }  else if (QString::compare(query_type, QString("DELETE"), Qt::CaseInsensitive) == 0) {

    } else {
        // other queries

    }

}
