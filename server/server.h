#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QMap>
#include "database_connector.h"
#include "client.h"

class Server : public QTcpServer
{
public:
    explicit Server(QObject *parent = 0);

    bool startServer(QHostAddress addr, quint16 port);
    void sendMessageToUser(QString msg, unsigned int id);
    bool regNewUser(QString login, QString password, QString name);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QMap<unsigned int, Client*> _onlineClients;

};

#endif // SERVER_H
