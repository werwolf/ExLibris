#ifndef EDBCONNECTION_H
#define EDBCONNECTION_H

#include <QSqlDatabase>
#include <QtCore>

class EDBconnection : public QObject
{
    Q_OBJECT

public:
    static EDBconnection* getInstance();
//    static void DestroyInstance (void) { if (pinstance) delete pinstance; } ;
    ~EDBconnection();
    void executeSqlQuery(const QString) const;

public slots:
    void checkUser(QString login, QString pwd = "ANY");

signals:
    void setUserId(long);

protected:
    EDBconnection();
    EDBconnection( const EDBconnection& );
    EDBconnection& operator=( const EDBconnection& );

private:
    static EDBconnection* pinstance;

    QSqlDatabase db;
    QString dbHost;
    QString dbName;
    QString dbUser;
    QString dbPass;
};

#endif // EDBCONNECTION_H
