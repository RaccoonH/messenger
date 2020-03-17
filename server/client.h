#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include "listofcommands.h"
#include "database_connector.h"

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(qintptr desc, QObject *parent);

private slots:
    void onConnect();
    void onDisconnect();
    void onReadyRead();
//    void onError(QAbstractSocket::SocketError socketError) const;


private:
    QTcpSocket *_sock;

    quint16 _blockSize;

    void sendCommandToClient(qint8 comm, int userID = -1);
};

#endif // CLIENT_H
