#include "client.h"

Client::Client(qintptr desc, QObject *parent) : QObject(parent)
{
    _sock = new QTcpSocket(this);
    _sock->setSocketDescriptor(desc);
    _blockSize = 0;

    connect(_sock, &QTcpSocket::connected, this, &Client::onConnect);
    connect(_sock, &QTcpSocket::disconnected, this, &Client::onDisconnect);
    connect(_sock, &QTcpSocket::readyRead, this, &Client::onReadyRead);
//    connect(_sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
}

void Client::onConnect()
{

}

void Client::onDisconnect()
{

}

void Client::onReadyRead()
{
    QDataStream in(_sock);
    if (_blockSize == 0) {
        if (_sock->bytesAvailable() < (int)sizeof(quint16))
        {
            return;
        }
        in >> _blockSize;
        qDebug() << "_blockSize now " << _blockSize;
    }
    if (_sock->bytesAvailable() < _blockSize)
    {
        return;
    }
    else
    {
        _blockSize = 0;
    }
    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;

    switch (command)
    {
        case ServerCommand::clientRegistration:
        {
            QString login;
            QString password;
            QString name;
            in >> login;
            in >> password;
            in >> name;
            qDebug() << login;
            qDebug() << password;
            qDebug() << name;

            bool registSuccess;
            registSuccess = DataBaseConnector::getInstance()->addNewUser(login, password, name);
            if(registSuccess)
            {
                qint8 comm = ServerCommand::servRegSuccess;
                sendCommandToClient(comm);
            }
            else
            {
                qint8 comm = ServerCommand::errorRegLogin;
                sendCommandToClient(comm);
            }

            break;
        }

        case ServerCommand::clientAuthorization:
        {
            QString login;
            QString password;
            in >> login;
            in >> password;

            int idClient;
            idClient = DataBaseConnector::getInstance()->authUser(login, password);
            if(idClient != -1)
            {
                qint8 comm = ServerCommand::servAuthSuccess;
                sendCommandToClient(comm, idClient);

            }
            else
            {
                qint8 comm = ServerCommand::errorAuth;
                sendCommandToClient(comm);
            }
            break;
        }

        case ServerCommand::clientSendMessageToUser:
        {
            unsigned int userID;
            in >> userID;
            QString message;
            in >> message;

            break;
        }
    }
}

void Client::sendCommandToClient(qint8 comm, int userID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << comm;
    if(userID != -1)
        out << userID;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}
