#ifndef DATABASE_CONNECTOR_H
#define DATABASE_CONNECTOR_H

#include <QString>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "message.h"

class DataBaseConnector
{
public:
    static DataBaseConnector* getInstance();
    static void deinit();

    bool addNewUser(QString login, QString password, QString name);
    int authUser(QString login, QString password);
    QList<Message> getDialogMessages(unsigned int idUser1, unsigned int idUser2);
    void addNewDialogMessage(unsigned int idUser1, unsigned int idUser2, QString text);

private:
    static DataBaseConnector *_instance;
    QSqlDatabase _dataBase;

    DataBaseConnector();
    bool connectToDataBase();
};

#endif // DATABASE_CONNECTOR_H
