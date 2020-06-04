#ifndef DATABASE_CONNECTOR_H
#define DATABASE_CONNECTOR_H

#include <QString>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "data_struct.h"

class DataBaseConnector
{
public:
    static DataBaseConnector* getInstance();
    static void deinit();

    bool addNewUser(QString login, QString password, QString name);
    int authUser(QString login, QString password);
    advanUserInfo getAuthClientInfo(unsigned int id);
    QList<baseClientInfo> getClientList(unsigned int id);

    //QList<Message> getAllMessages(unsigned int id);

    QList<unsigned int> getAdminsChannel(unsigned int userID);
    QList<unsigned int> getAdminsGroup(unsigned int userID);

    QList<messageStruct> getDialogMessages(unsigned int idUser1, unsigned int idUser2);
    QList<messageStruct> getGroupMessages(unsigned int groupID);
    QList<messageStruct> getChannelMessages(unsigned int channelID);

    void addNewDialogMessage(unsigned int senderID, unsigned int reciverID, QString text, QDateTime time);
    QList<unsigned int> addNewGroupMessage(unsigned int senderID, unsigned int groupID, QString text, QDateTime time);
    QList<unsigned int> addNewChannelMessage(unsigned int channelID, QString text, QDateTime time);

    advanUserInfo getUserInfo(unsigned int id);
    advanChannelInfo getChannelInfo(unsigned int id);
    advanGroupInfo getGroupInfo(unsigned int id);
    QList<baseClientInfo> getListOfGroupUsers(unsigned int groupID);

    QList<baseClientInfo> getListOfFriends(unsigned int id);
    void addNewUserInfo(unsigned int id, advanUserInfo info);

    QList<baseClientInfo> getListOfChannelSubs(unsigned int id);
    void deleteSubFromChannel(unsigned int userID, unsigned int channelID);
    void deleteUserFromGroup(unsigned int userID, unsigned int groupID);
    void addNewChannelInfo(unsigned int id, advanChannelInfo info);
    void addNewGroupInfo(unsigned int id, advanGroupInfo info);
    unsigned int inviteUserToGroup(QString login, unsigned int groupID);
    unsigned int inviteUserToChannel(QString login, unsigned int groupID);
    bool isExistUser(QString login);
    bool isChannelLoginExistAlready(QString login);
    unsigned int createNewChannel(advanChannelInfo info, unsigned int adminID, QList<unsigned int > invitedUsers);
    bool isGroupLoginExistAlready(QString login);
    unsigned int createNewGroup(advanGroupInfo info, unsigned int adminID, QList<unsigned int> invitedUsers);
    void deleteChannel(unsigned int id);
    void deleteGroup(unsigned int id);
    void leaveChannel(unsigned int channelID, unsigned int userID);
    QList<advanUserInfo> searchUsers(QString login);
    QList<advanChannelInfo> searchChannels(QString login);
    QList<advanGroupInfo> searchGroups(QString login);
    void startNewDialog(unsigned int id, unsigned int initUserID);
    void joinToChannel(unsigned int channelID, unsigned int userID);
    void joinToGroup(unsigned int groupID, unsigned int userID);

    unsigned int deleteDialogMessage(unsigned int senderID, QDateTime date, QString text);
    void deleteChannelMessage(unsigned int senderID, QDateTime, QString text);
    void deleteGroupMessage(unsigned int senderID, QDateTime, QString text);

    void editMessage(unsigned int senderID, QDateTime date, QString text, QString newText);
    void addFriend(unsigned int invitedID, unsigned int id);
    void removeFriend(unsigned int removedID, unsigned int id);

private:
    static DataBaseConnector *_instance;
    QSqlDatabase _dataBase;

    DataBaseConnector();
    bool connectToDataBase();
};

#endif // DATABASE_CONNECTOR_H
