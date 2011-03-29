#ifndef EDBCONNECTION_H
#define EDBCONNECTION_H

#include <QSqlDatabase>
#include <QtCore>

class EDBconnection : public QObject
{
    Q_OBJECT

public:
    static EDBconnection* getInstance();
//    static void destroyInstance();
    ~EDBconnection();

    static QString escape(const QString q);
    bool query(const QString) const;
    int insert(const QString) const;
    QList<QStringList> get(const QString) const;

    QString get_dbHost(void) const { return dbHost; }
    QString get_dbName(void) const { return dbName; }
    QString get_dbUser(void) const { return dbUser; }
    QString get_dbPass(void) const { return dbPass; }

public slots:
//    void query(const QString) const;
//    void select(const QString);// const;
    void checkUser(QString login, QString pwd = "ANY");

//  newUser(login, pass, lastname, name, address, phone, email type);
    int newUser(QString, QString, QString, QString, QString, QString, QString, QString);
    void newAuthor(QString, QString, QString, QString, QDate, QString, QString, QString, QString);
    void newClient(QString, QString, QString, QString, QString, QString, QString, QString);
    void newSupplier(QString, QString, QString, QString, unsigned int, QString, QString, QString, QString);

signals:
    void setUserId(long);
    void returnLastError(QString);
//    void returnSelQuery(QList<QStringList>);

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

//    QString lastError;
};

#endif // EDBCONNECTION_H
