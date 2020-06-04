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

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    void onUserDisconnected(Client*);
    void onUserLoggined(unsigned int id, Client* client, advanUserInfo info);
    void onSendedDialogMessage(unsigned int senderID, unsigned int reciverID, messageStruct msg);
    void onSendedGroupMessage(QList<unsigned int> userList, unsigned int groupID, messageStruct msg);
    void onSendedChannelMessage(QList<unsigned int> subsList, unsigned int channelID, messageStruct msg);
    void onSendedNewUserInfo(QList<unsigned int> userList, advanUserInfo info, unsigned int id);
    void onSendedNewChannelInfo(QList<baseClientInfo> subsList, advanChannelInfo info);
    void onSendedNewGroupInfo(QList<baseClientInfo> usersList, advanGroupInfo info);

    void onDeletedSubFromChannel(QList<baseClientInfo> subList, unsigned int userID, unsigned int channelID);
    void onDeletedUserFromGroup(QList<baseClientInfo> userList, unsigned int userID, unsigned int groupId);

    void onInvitedUserToGroup(QList<baseClientInfo> userList, advanUserInfo userInfo, unsigned int groupID, QString groupName);
    void onInvitedUserToChannel(QList<baseClientInfo> userList, advanUserInfo userInfo, unsigned int channelID, QString channelName);
    void onInvitedFriendsToNewGroup(QList<unsigned int> friendList, advanGroupInfo groupInfo, QList<baseClientInfo> list);
    void onInvitedFriendsToNewChannel(QList<unsigned int> friendList, advanChannelInfo groupInfo, QList<baseClientInfo> list, unsigned int adminID);

    void onDeletedGroup(unsigned int id, QList<baseClientInfo> listOfUser);
    void onDeletedChannel(unsigned int id, QList<baseClientInfo> listOfUser);
    void onLeavedFromGroup(unsigned int userID, unsigned int groupID, QList<baseClientInfo> listOfUser);
    void onLeavedFromChannel(unsigned int userID, unsigned int channelID, QList<baseClientInfo> listOfUser);

    void onDeletedMessage(unsigned int senderID, QDateTime date, int type, QList<baseClientInfo> listOfUser, unsigned int groupID);
    void onChangedMessage(unsigned int senderID, QString newText, QDateTime date, int type, QList<baseClientInfo> listOfUser, unsigned int groupID);
    void onStartedNewDialog(unsigned int id, advanUserInfo info);
    void onAddedToFriendsList(unsigned int invitedID, advanUserInfo info);
    void onDeletedFriend(unsigned int removedID, unsigned int id);

    QMap<unsigned int, Client*> _onlineClients;

};

#endif // SERVER_H
