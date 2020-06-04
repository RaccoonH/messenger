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
    connect(client, &Client::loggined, this, &Server::onUserLoggined);
    connect(client, &Client::disconnected, this, &Server::onUserDisconnected);
    connect(client, &Client::sendedDialogMessage, this, &Server::onSendedDialogMessage);
    connect(client, &Client::sendedGroupMessage, this, &Server::onSendedGroupMessage);
    connect(client, &Client::sendedChannelMessage, this, &Server::onSendedChannelMessage);
    connect(client, &Client::sendedNewUserInfo, this, &Server::onSendedNewUserInfo);
    connect(client, &Client::sendedNewChannelInfo, this, &Server::onSendedNewChannelInfo);
    connect(client, &Client::sendedNewGroupInfo, this, &Server::onSendedNewGroupInfo);

    connect(client, &Client::deletedSubFromChannel, this, &Server::onDeletedSubFromChannel);
    connect(client, &Client::deletedUserFromGroup, this, &Server::onDeletedUserFromGroup);
    connect(client, &Client::invitedUserToGroup, this, &Server::onInvitedUserToGroup);
    connect(client, &Client::invitedUserToChannel, this, &Server::onInvitedUserToChannel);
    connect(client, &Client::invitedFriendsToNewGroup, this, &Server::onInvitedFriendsToNewGroup);
    connect(client, &Client::invitedFriendsToNewChannel, this, &Server::onInvitedFriendsToNewChannel);

    connect(client, &Client::deletedGroup, this, &Server::onDeletedGroup);
    connect(client, &Client::deletedChannel, this, &Server::onDeletedChannel);
    connect(client, &Client::leavedFromGroup, this, &Server::onLeavedFromGroup);
    connect(client, &Client::leavedFromChannel, this, &Server::onLeavedFromChannel);
    connect(client, &Client::deletedMessage, this, &Server::onDeletedMessage);
    connect(client, &Client::changedMessage, this, &Server::onChangedMessage);
    connect(client, &Client::startedNewDialog, this, &Server::onStartedNewDialog);
    connect(client, &Client::addedToFriendsList, this, &Server::onAddedToFriendsList);
    connect(client, &Client::deletedFriend, this, &Server::onDeletedFriend);

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

void Server::onUserLoggined(unsigned int id, Client* client, advanUserInfo info)
{
    if(_onlineClients.contains(id))
    {
        client->sendCommandToClient(ServerCommand::errorAuth);
    }
    else
    {
        _onlineClients.insert(id, client);
        client->sendAuthClientInfo(id, info);
    }
    qDebug() << id;
}

void Server::onSendedDialogMessage(unsigned int senderID, unsigned int reciverID, messageStruct msg)
{
    if(_onlineClients.contains(reciverID))
    {
        qDebug() << "YES";
        _onlineClients.value(reciverID)->sendDialogMessage(senderID, msg);
    }
}

void Server::onSendedGroupMessage(QList<unsigned int> userList, unsigned int groupID, messageStruct msg)
{
    for (int i = 0; i < userList.size(); ++i)
    {
        if(_onlineClients.contains(userList[i]))
        {
            qDebug() << "YES";
            _onlineClients.value(userList[i])->sendGroupMessage(groupID, msg);
        }
    }
}

void Server::onSendedChannelMessage(QList<unsigned int> subsList, unsigned int channelID, messageStruct msg)
{
    for (int i = 0; i < subsList.size(); ++i)
    {
        if(_onlineClients.contains(subsList[i]))
        {
            qDebug() << "YES";
            _onlineClients.value(subsList[i])->sendChannelMessage(channelID, msg);
        }
    }
}

void Server::onSendedNewUserInfo(QList<unsigned int> userList, advanUserInfo info, unsigned int id)
{
    for (int i = 0; i < userList.count(); i++)
    {
        if(_onlineClients.contains(userList[i]))
        {
            _onlineClients.value(userList[i])->sendNewUserInfo(id, info);
        }
    }
}

void Server::onSendedNewChannelInfo(QList<baseClientInfo> subsList, advanChannelInfo info)
{
    for (int i = 0; i < subsList.count(); i++)
    {
        if(_onlineClients.contains(subsList[i].id))
        {
            _onlineClients.value(subsList[i].id)->sendNewChannelInfo(info);
        }
    }
}

void Server::onSendedNewGroupInfo(QList<baseClientInfo> usersList, advanGroupInfo info)
{
    for (int i = 0; i < usersList.count(); i++)
    {
        if(_onlineClients.contains(usersList[i].id))
        {
            _onlineClients.value(usersList[i].id)->sendNewGroupInfo(info);
        }
    }
}

void Server::onDeletedSubFromChannel(QList<baseClientInfo> subList, unsigned int userID, unsigned int channelID)
{
    for(int i = 0; i < subList.count(); i++)
    {
        if(_onlineClients.contains(subList[i].id))
        {
            _onlineClients.value(subList[i].id)->deleteSubFromChannel(userID, channelID);
        }
    }
}

void Server::onDeletedUserFromGroup(QList<baseClientInfo> userList, unsigned int userID, unsigned int groupId)
{
    for(int i = 0; i < userList.count(); i++)
    {
        if(_onlineClients.contains(userList[i].id))
        {
            _onlineClients.value(userList[i].id)->deleteUserFromGroup(userID, groupId);
        }
    }
}

void Server::onInvitedUserToGroup(QList<baseClientInfo> userList, advanUserInfo userInfo, unsigned int groupID, QString groupName)
{
    for(int i = 0; i < userList.count(); i++)
    {
        if(_onlineClients.contains(userList[i].id))
        {
            _onlineClients.value(userList[i].id)->sendInvitedUserInfo(userInfo, groupID, groupName);
        }
    }
}

void Server::onInvitedUserToChannel(QList<baseClientInfo> userList, advanUserInfo userInfo, unsigned int channelID, QString channelName)
{
    for(int i = 0; i < userList.count(); i++)
    {
        if(_onlineClients.contains(userList[i].id))
        {
            _onlineClients.value(userList[i].id)->sendInvitedUserInfoChannel(userInfo, channelID, channelName);
        }
    }
}

void Server::onInvitedFriendsToNewGroup(QList<unsigned int> friendList, advanGroupInfo groupInfo, QList<baseClientInfo> list)
{
    for(int i = 0; i < friendList.count(); i++)
    {
        if(_onlineClients.contains(friendList[i]))
        {
            _onlineClients.value(friendList[i])->sendNewGroup(groupInfo, list);
        }
    }
}

void Server::onInvitedFriendsToNewChannel(QList<unsigned int> friendList, advanChannelInfo groupInfo, QList<baseClientInfo> list, unsigned int adminID)
{
    for(int i = 0; i < friendList.count(); i++)
    {
        if(_onlineClients.contains(friendList[i]))
        {
            _onlineClients.value(friendList[i])->sendNewChannel(groupInfo, list, adminID);
        }
    }
}

void Server::onDeletedGroup(unsigned int id, QList<baseClientInfo> listOfUser)
{
    for(int i = 0; i < listOfUser.count(); i++)
    {
        if(_onlineClients.contains(listOfUser[i].id))
        {
            _onlineClients.value(listOfUser[i].id)->sendDeletedGroup(id);
        }
    }
}

void Server::onDeletedChannel(unsigned int id, QList<baseClientInfo> listOfUser)
{
    for(int i = 0; i < listOfUser.count(); i++)
    {
        if(_onlineClients.contains(listOfUser[i].id))
        {
            _onlineClients.value(listOfUser[i].id)->sendDeletedChannel(id);
        }
    }
}

void Server::onLeavedFromGroup(unsigned int userID, unsigned int groupID, QList<baseClientInfo> listOfUser)
{
    for(int i = 0; i < listOfUser.count(); i++)
    {
        if(_onlineClients.contains(listOfUser[i].id))
        {
            _onlineClients.value(listOfUser[i].id)->sendLeavedFromGroup(userID, groupID);
        }
    }
}

void Server::onLeavedFromChannel(unsigned int userID, unsigned int channelID, QList<baseClientInfo> listOfUser)
{
    for(int i = 0; i < listOfUser.count(); i++)
    {
        if(_onlineClients.contains(listOfUser[i].id))
        {
            _onlineClients.value(listOfUser[i].id)->sendLeavedFromChannel(userID, channelID);
        }
    }
}

void Server::onDeletedMessage(unsigned int senderID, QDateTime date, int type, QList<baseClientInfo> listOfUser, unsigned int groupID)
{
    for(int i = 0; i < listOfUser.count(); i++)
    {
        if(_onlineClients.contains(listOfUser[i].id))
        {
            _onlineClients.value(listOfUser[i].id)->sendDeletedMessage(senderID, date, type, groupID);
        }
    }
}

void Server::onChangedMessage(unsigned int senderID, QString newText, QDateTime date, int type, QList<baseClientInfo> listOfUser, unsigned int groupID)
{
    for(int i = 0; i < listOfUser.count(); i++)
    {
        if(_onlineClients.contains(listOfUser[i].id))
        {
            _onlineClients.value(listOfUser[i].id)->sendChangedMessage(senderID, newText, date, type, groupID);
        }
    }
}

void Server::onStartedNewDialog(unsigned int id, advanUserInfo info)
{
    if(_onlineClients.contains(id))
    {
        _onlineClients.value(id)->sendNewDialog(info);
    }
}

void Server::onAddedToFriendsList(unsigned int invitedID, advanUserInfo info)
{
    if(_onlineClients.contains(invitedID))
    {
        _onlineClients.value(invitedID)->sendAddedToFriendsList(info);
    }
}

void Server::onDeletedFriend(unsigned int removedID, unsigned int id)
{
    if(_onlineClients.contains(removedID))
    {
        _onlineClients.value(removedID)->sendDeletedFriend(id);
    }
}

void Server::onUserDisconnected(Client* client)
{
    unsigned int id = _onlineClients.key(client);
    _onlineClients.remove(id);
    client->deleteLater();
}

void Server::sendMessageToUser(QString msg, unsigned int id)
{
    ///workWithDataBase;
}

