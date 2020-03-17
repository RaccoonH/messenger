#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    DataBaseConnector::getInstance();
}

bool Server::startServer(QHostAddress addr, quint16 port)
{
    if (!listen(addr, port))
    {
        qDebug() << "Server not started at" << addr << ":" << port;
        return false;
    }
    qDebug() << "Server started at" << addr << ":" << port;
    return true;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    Client *client = new Client(socketDescriptor, this);
    qDebug() << socketDescriptor;
}

//bool Server::regNewUser(QString login, QString password, QString name)
//{
//    bool regSuccess;
//    regSuccess = DataBaseConnector::getInstance()->addNewUser(login, password, name);
//    if(regSuccess)
//    {

//    }
//}

void Server::sendMessageToUser(QString msg, unsigned int id)
{
    ///workWithDataBase;
}

