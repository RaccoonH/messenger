#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QThread>
#include "listofcommands.h"
#include "database_connector.h"
#include "data_struct.h"

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(qintptr desc, QObject *parent);
    ~Client();

    void sendCommandToClient(qint8 comm);

    void sendAuthClientInfo(int id, advanUserInfo);
    void sendDialogMessage(unsigned int reciverID, messageStruct msg);
    void sendGroupMessage(unsigned int groupID, messageStruct msg);
    void sendChannelMessage(unsigned int channelID, messageStruct msg);

    void sendNewUserInfo(unsigned int id, advanUserInfo userInfo);
    void sendNewChannelInfo(advanChannelInfo channelInfo);
    void sendNewGroupInfo(advanGroupInfo channelInfo);

    void deleteSubFromChannel(unsigned int userID, unsigned int channelID);
    void deleteUserFromGroup(unsigned int userID, unsigned int groupID);

    void sendInvitedUserInfo(advanUserInfo info, unsigned int groupID, QString groupName);
    void sendInvitedUserInfoChannel(advanUserInfo info, unsigned int channelID, QString channelName);
    void sendNewChannel(advanChannelInfo info, QList<baseClientInfo> listOfUsers, unsigned int id);
    void sendNewGroup(advanGroupInfo info, QList<baseClientInfo> listOfUsers);

    void sendDeletedGroup(unsigned int id);
    void sendDeletedChannel(unsigned int id);
    void sendLeavedFromGroup(unsigned int userID, unsigned int groupID);
    void sendLeavedFromChannel(unsigned int userID, unsigned int channelID);
    void sendDeletedMessage(unsigned int senderID, QDateTime date, int type, unsigned int groupID);
    void sendChangedMessage(unsigned senderID,  QString newText, QDateTime date, int type, unsigned int groupID);
    void sendNewDialog(advanUserInfo info);
    void sendAddedToFriendsList(advanUserInfo info);
    void sendDeletedFriend(unsigned int id);

private slots:
    void onConnect();
    void onDisconnect();
    void onReadyRead();
//    void onError(QAbstractSocket::SocketError socketError) const;

signals:
    void disconnected(Client*);
    void loggined(unsigned int id, Client*, advanUserInfo info);
    void sendedDialogMessage(unsigned int senderID, unsigned int reciverID, messageStruct msg);
    void sendedGroupMessage(QList<unsigned int> userList, unsigned int groupID, messageStruct msg);
    void sendedChannelMessage(QList<unsigned int> subsList, unsigned int channelID, messageStruct msg);

    void sendedNewUserInfo(QList<unsigned int> usersList, advanUserInfo info, unsigned int id);
    void sendedNewChannelInfo(QList<baseClientInfo> subsList, advanChannelInfo info);
    void sendedNewGroupInfo(QList<baseClientInfo> userList, advanGroupInfo info);

    void deletedSubFromChannel(QList<baseClientInfo> subList, unsigned int userID, unsigned int channelID);
    void deletedUserFromGroup(QList<baseClientInfo> userList, unsigned int userID, unsigned int groupID);

    void invitedUserToGroup(QList<baseClientInfo> userList, advanUserInfo userInfo, unsigned int groupID, QString groupName);
    void invitedUserToChannel(QList<baseClientInfo> userList, advanUserInfo userInfo, unsigned int groupID, QString groupName);
    void invitedFriendsToNewGroup(QList<unsigned int> friendList, advanGroupInfo groupInfo, QList<baseClientInfo> list);
    void invitedFriendsToNewChannel(QList<unsigned int> friendList, advanChannelInfo channelInfo, QList<baseClientInfo> list, unsigned int adminID);

    void deletedGroup(unsigned int id, QList<baseClientInfo> listOfUser);
    void deletedChannel(unsigned int id, QList<baseClientInfo> listOfUser);
    void leavedFromGroup(unsigned int userID, unsigned int groupID, QList<baseClientInfo> listOfUser);
    void leavedFromChannel(unsigned int userID, unsigned int groupID, QList<baseClientInfo> listOfUser);
    void deletedMessage(unsigned int senderID, QDateTime date, int type, QList<baseClientInfo> listOfUser, unsigned int groupID = 0);
    void changedMessage(unsigned int senderID, QString newText,QDateTime date, int type, QList<baseClientInfo> listOfUser, unsigned int groupID = 0);
    void startedNewDialog(unsigned int id, advanUserInfo info);
    void addedToFriendsList(unsigned int invitedID, advanUserInfo info);
    void deletedFriend(unsigned int removedID, unsigned int id);


private:
    QTcpSocket *_sock;

    quint16 _blockSize;

    void sendBaseClientInfo(unsigned int userID, QList<baseClientInfo> list);

    void sendDialogMessagesList(unsigned int userID, QList<messageStruct> list);
    void sendGroupMessagesList(unsigned int groupID, QList<messageStruct> list);
    void sendChannelMessagesList(unsigned int channelID, QList<messageStruct> list);


    void sendUserInfo(unsigned int id, advanUserInfo userInfo);
    void sendChannelInfo(unsigned int id, advanChannelInfo channelInfo);
    void sendGroupInfo(unsigned int id, advanGroupInfo groupInfo, QList<baseClientInfo> listOfUsers, QList<advanUserInfo> usersInfo);

    void sendListOfFriends(QList<baseClientInfo>);
    void sendListOfChannelSubs(unsigned int channelID, QList<baseClientInfo>);
    void sendListOfGroupUsers(unsigned int groupID, QList<baseClientInfo>);

    void sendSearch(QList<advanUserInfo> userList, QList<advanChannelInfo> channelList, QList<advanGroupInfo> groupList);


};

#endif // CLIENT_H
