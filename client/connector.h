#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QTcpSocket>
#include <QList>
#include <QHostAddress>
#include "listofcommands.h"
#include "user.h"
#include "chat.h"
#include "userinfo.h"
#include "channel.h"
#include "channelinfo.h"
#include "group.h"
#include "groupinfo.h"
#include "data_struct.h"

class Connector : public QObject
{
    Q_OBJECT

public:
    static Connector* getInstance();
    static void deinit();

	bool isLogined();

    void registUser(QString login, QString password, QString name);
    void loginUser(QString login, QString password);
    void loadClientList();

    void loadDialogMessages(unsigned int id);
    void loadGroupMessages(unsigned int id);
    void loadChannelMessages(unsigned int id);

    void sendDialogMessage(unsigned int reciverID, QString text, QDateTime time);
    void sendGroupMessage(unsigned int groupID, QString text, QDateTime time);
    void sendChannelMessage(unsigned int channelID, QString text, QDateTime time);

    void loadUserInfo(unsigned int id);
    void loadChannelInfo(unsigned int id);
    void loadGroupInfo(unsigned int id);

    bool isAdminOfChannel(unsigned int channelID);
    bool isAdminOfGroup(unsigned int groupID);

    void loadListOfFriends();
    void sendNewUserInfo(advanUserInfo info);

    void loadChannelSubs(unsigned int id);
    void loadGroupUsers(unsigned int id);
    void deleteSubFromChannel(unsigned int userID, unsigned int channelID);
    void deleteUserFromGroup(unsigned int userID, unsigned int groupID);
    void sendNewChannelInfo(unsigned int channelID, advanChannelInfo info);
    void sendNewGroupInfo(unsigned int channelID, advanGroupInfo info);
    void inviteUserToGroup(QString login, unsigned int groupID);
	void inviteUserToChannel(QString login, unsigned int channelID);
    void sendNewChannel(advanChannelInfo info, QList<unsigned int> list);
    void sendNewGroup(advanGroupInfo info, QList<unsigned int> list);
    void deleteChannel(unsigned int id);
    void deleteGroup(unsigned int id);
	void leaveChannel(unsigned int id);
	void leaveGroup(unsigned int id);
	void search(QString login, bool users, bool channels, bool groups);
	void startNewDialog(unsigned int id);
	void joinToChannel(unsigned int id);
	void joinToGroup(unsigned int id);
	void deleteMessage(Message *msg, int type, unsigned int id);
	void editMessage(Message *msg, QString newText, int type, unsigned int id);
	void addToFriendsList(unsigned int id);
	void removeFromFriends(unsigned int id);

    UserInfo* getCurrentUserInfo();
    User* getUser(unsigned int id);
    User* getUser(QString login);
    Group* getGroup(unsigned int id);
    Channel* getChannel(unsigned int id);
    QList<Client*> getListOfClient();
    QList<User*> getListOfFriends();
    int getCountOfClient();
	QList<Client*> getFoundClients();
	bool isSubOfChannel(unsigned int id);
	bool isUserOfGroup(unsigned int id);
	bool hasDialogWithUser(unsigned int id);
	bool isFriend(unsigned int id);
	void addNewDialogUser(unsigned int id);
	void addNewGroupJoin(unsigned int id);
	void addNewChannelJoin(unsigned int id);
	void errorChecked();
	bool isErrorFromServer();
	unsigned int getNewGroupID();

	bool logined = false;

	void deleteClientFromList(Client*);
	void deleteClientFromList(unsigned int id, int type);

public slots:
    void onSokReadyRead();

signals:
    void registered();
    void loadedClientList();
    void loggedIn();

	void loadedNewClientInfo(unsigned int id, int type);

    void loadedDialogChat(unsigned int id);
    void loadedGroupChat(unsigned int id);
    void loadedChannelChat(unsigned int id);

    void loadedNewDialogMessage(unsigned int id);
    void loadedNewGroupMessage(unsigned int id);
    void loadedNewChannelMessage(unsigned int id);

    void loadedUserInfo(unsigned int id);
    void loadedChannelInfo(unsigned int id);
    void loadedGroupInfo(unsigned int id);

    void loadedListOfFriends();

	void deletedFromClient(unsigned int id, int type);
	void deletedClient(unsigned int id, int type);
	void deletedMessage(unsigned int senderID, QDateTime date);
	void changedMessage(Message*);
	void addedByClient(unsigned int id, ClientInfo* info);

    void errRegistLoginExist();
    void errLogin();


private:
    Connector();
    void connectToServer();

    static Connector *instance;
    QTcpSocket *_socket;
    quint16 _blockSize;
    UserInfo *_userInfo = nullptr;
    bool isConnected;
	bool errorFromServer = false;
	unsigned int newGroupID = 0;
    //QMap<unsigned int, Client*> mapOfClient;
    QList<Client*> _listOfClient;
    QList<User*> _listOfFriends;
    QList<unsigned int> _listOfAdminChannel;
    QList<unsigned int> _listOfAdminGroup;
	QList<unsigned int> _listOfChannel;
	QList<unsigned int> _listOfGroup;
	QList<unsigned int> _listOfDialogsUsers;
	QList<Client*> _foundClients;

    //QList<baseClientInfo> _baseClientInfoList;

    //void addClientToMap(unsigned int id, Client* client);

};

#endif // CONNECTOR_H
