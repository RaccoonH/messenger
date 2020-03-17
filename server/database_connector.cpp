#include "database_connector.h"

DataBaseConnector* DataBaseConnector::_instance = 0;

DataBaseConnector::DataBaseConnector()
{

}

DataBaseConnector* DataBaseConnector::getInstance()
{
    if(_instance == 0)
    {
        _instance = new DataBaseConnector();
        if(_instance->connectToDataBase())
        {

        }
    }

    return _instance;
}

bool DataBaseConnector::connectToDataBase()
{
    _dataBase = QSqlDatabase::addDatabase("QSQLITE");
    if(QFile("messengerDB.db").exists())
    {
        _dataBase.setDatabaseName("messengerDB.db");
        _dataBase.open();

        return true;
    }

    else
    {
        return false;
    }
}

//QList<Message> DataBaseConnector::getDialogMessages(unsigned int idUser1, unsigned int idUser2)
//{
//    QSqlQuery query;
//    query.prepare("SELECT Text, senderID FROM Message WHERE DialogMsg.MessageID = Message.id");
//}

bool DataBaseConnector::addNewUser(QString login, QString password, QString name)
{
    QSqlQuery queryCheckLogin;
    queryCheckLogin.prepare("SELECT id FROM User WHERE :login = login");
    queryCheckLogin.bindValue(":login", login);
    queryCheckLogin.exec();
    if(!queryCheckLogin.next())
    {
        QSqlQuery queryInsertNewUser;
        queryInsertNewUser.prepare("INSERT INTO User (id, login, password, name) VALUES (NULL, :login, :password, :name)");
        queryInsertNewUser.bindValue(":login", login);
        queryInsertNewUser.bindValue(":password", password);
        queryInsertNewUser.bindValue(":name", name);
        queryInsertNewUser.exec();
        return true;
    }
    else
    {
        return false;
    }
}

int DataBaseConnector::authUser(QString login, QString password)
{
    QSqlQuery queryCheckLoginAndPassword;
    queryCheckLoginAndPassword.prepare("SELECT id FROM User WHERE :login = login AND :password = password");
    queryCheckLoginAndPassword.bindValue(":login", login);
    queryCheckLoginAndPassword.bindValue(":password", password);
    queryCheckLoginAndPassword.exec();
    if(queryCheckLoginAndPassword.next())
    {
        return queryCheckLoginAndPassword.value(0).toInt();
    }
    else
    {
        return -1;
    }

}

void DataBaseConnector::addNewDialogMessage(unsigned int idUser1, unsigned int idUser2, QString text)
{
    QSqlQuery queryAddNewMessage;
    queryAddNewMessage.prepare("INSERT INTO Message (Text, SenderID) VALUES (:Text, :SenderID)");
    queryAddNewMessage.bindValue(":Text", text);
    queryAddNewMessage.bindValue(":SenderID", idUser1);
    queryAddNewMessage.exec();

    QSqlQuery queryGetID;
    queryGetID.prepare("SELECT last_insert_rowid()");
    queryGetID.exec();
    queryGetID.next();
    int idMsg = queryGetID.value(0).toInt();

    QSqlQuery queryAddDialogMessage;
    queryAddDialogMessage.prepare("INSERT INTO DialogMsg (id, idUser1, idUser2) VALUES (:id, :idUser1, :idUser2)");
    queryAddDialogMessage.bindValue(":id", idMsg);
    queryAddDialogMessage.bindValue(":idUser1", idUser1);
    queryAddDialogMessage.bindValue(":idUser2", idUser2);
    queryAddDialogMessage.exec();
}
