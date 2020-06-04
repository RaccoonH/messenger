#include "database_connector.h"
#include <QDebug>

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

advanUserInfo DataBaseConnector::getAuthClientInfo(unsigned int id)
{
    QSqlQuery queryGetAuthClientInfo;
    //need more info maybe
    queryGetAuthClientInfo.prepare("SELECT name, login, description, number FROM User WHERE :userID = id");
    queryGetAuthClientInfo.bindValue(":userID", id);
    queryGetAuthClientInfo.exec();
    queryGetAuthClientInfo.next();

    advanUserInfo authClientInfo;
    authClientInfo.name = queryGetAuthClientInfo.value(0).toString();
    authClientInfo.login = queryGetAuthClientInfo.value(1).toString();
    authClientInfo.desc = queryGetAuthClientInfo.value(2).toString();
    authClientInfo.number = queryGetAuthClientInfo.value(3).toString();

    return authClientInfo;
}

QList<baseClientInfo> DataBaseConnector::getClientList(unsigned int id)
{
    QList<baseClientInfo> list;

    QSqlQuery queryGetUsers;
    queryGetUsers.prepare("SELECT id, name FROM User WHERE id IN (SELECT userID1 FROM Dialog WHERE :id = userID2 union SELECT userID2 FROM Dialog WHERE :id = userID1)");
    queryGetUsers.bindValue(":id", id);
    queryGetUsers.exec();
    while(queryGetUsers.next())
    {
        baseClientInfo info;
        info.type = 1;
        info.id = queryGetUsers.value(0).toInt();
        info.name = queryGetUsers.value(1).toString();
        list.append(info);
    }

    QSqlQuery queryGetChannels;
    queryGetChannels.prepare("SELECT id, name FROM Channel WHERE id IN "
                             "(SELECT channelID FROM ChannelSubs WHERE :id = userID)");
    queryGetChannels.bindValue(":id", id);
    queryGetChannels.exec();
    while(queryGetChannels.next())
    {
        baseClientInfo info;
        info.type = 2;
        info.id = queryGetChannels.value(0).toInt();
        info.name = queryGetChannels.value(1).toString();
        list.append(info);
    }

    QSqlQuery queryGetGroups;
    queryGetGroups.prepare("SELECT id, name FROM [Group] WHERE id IN "
                           "(SELECT groupID FROM GroupUser WHERE :id = userID)");
    queryGetGroups.bindValue(":id", id);
    queryGetGroups.exec();
    while(queryGetGroups.next())
    {
        baseClientInfo info;
        info.type = 3;
        info.id = queryGetGroups.value(0).toInt();
        info.name = queryGetGroups.value(1).toString();
        list.append(info);
    }

    return list;
}

QList<unsigned int> DataBaseConnector::getAdminsChannel(unsigned int userID)
{
    QSqlQuery queryGetAdminsChannel;
    queryGetAdminsChannel.prepare("SELECT id FROM Channel WHERE :id = adminID");
    queryGetAdminsChannel.bindValue(":id", userID);
    queryGetAdminsChannel.exec();
    QList<unsigned int> list;
    while(queryGetAdminsChannel.next())
    {
        list.append(queryGetAdminsChannel.value(0).toInt());
    }

    return list;
}

QList<unsigned int> DataBaseConnector::getAdminsGroup(unsigned int userID)
{
    QSqlQuery queryGetAdminsGroup;
    queryGetAdminsGroup.prepare("SELECT id FROM [Group] WHERE :id = adminID");
    queryGetAdminsGroup.bindValue(":id", userID);
    queryGetAdminsGroup.exec();
    QList<unsigned int> list;
    while(queryGetAdminsGroup.next())
    {
        list.append(queryGetAdminsGroup.value(0).toInt());
    }

    return  list;
}

//QList<Message> DataBaseConnector::getAllMessages(unsigned int id)
//{
//    QSqlQuery queryGetAllDialogMsgs;
//    queryGetAllDialogMsgs.prepare("");
//}

QList<messageStruct> DataBaseConnector::getDialogMessages(unsigned int idUser1, unsigned int idUser2)
{
    QSqlQuery queryGetMessages;
    queryGetMessages.prepare("SELECT u.name, m.senderID, m.text, m.data FROM Message as m, User as u WHERE "
                             "m.senderID = u.id AND m.id IN (SELECT messageID FROM DialogMsg WHERE "
                             "dialogID = (SELECT dialogID FROM Dialog WHERE (userID1 = :user1 AND userID2 = :user2) "
                             "OR (userID1 = :user2 AND userID2 = :user1))) ORDER BY m.data ASC");
    queryGetMessages.bindValue(":user1", idUser1);
    queryGetMessages.bindValue(":user2", idUser2);
    queryGetMessages.exec();

    QList<messageStruct> messageList;
    while(queryGetMessages.next())
    {
        messageStruct msg;
        msg.senderName = queryGetMessages.value(0).toString();
        msg.senderID = queryGetMessages.value(1).toInt();
        msg.text = queryGetMessages.value(2).toString();
        msg.time = queryGetMessages.value(3).toDateTime();
        messageList.append(msg);
    }

    return messageList;
}

QList<messageStruct> DataBaseConnector::getGroupMessages(unsigned int groupID)
{
    QSqlQuery queryGetMessages;
    queryGetMessages.prepare("SELECT u.name, m.senderID, m.text, m.data FROM Message AS m, User AS u "
                             "WHERE m.senderID = u.id AND m.id "
                             "IN (SELECT messageID FROM GroupMsg WHERE :groupID = groupID) "
                             "ORDER BY m.data ASC");
    queryGetMessages.bindValue(":groupID", groupID);
    queryGetMessages.exec();
    QList<messageStruct> messageList;
    while(queryGetMessages.next())
    {
        messageStruct msg;
        msg.senderName = queryGetMessages.value(0).toString();
        msg.senderID = queryGetMessages.value(1).toInt();
        msg.text = queryGetMessages.value(2).toString();
        msg.time = queryGetMessages.value(3).toDateTime();
        messageList.append(msg);
    }

    return messageList;
}

QList<messageStruct> DataBaseConnector::getChannelMessages(unsigned int channelID)
{
    QSqlQuery queryGetMessages;
    queryGetMessages.prepare("SELECT c.name, c.id, m.text, m.data FROM Message AS m, Channel AS c "
                             "WHERE c.id = m.senderID AND m.id "
                             "IN (SELECT messageID FROM ChannelMsg WHERE :channelID = channelID) "
                             "ORDER BY m.data ASC");
    queryGetMessages.bindValue(":channelID", channelID);
    queryGetMessages.exec();
    QList<messageStruct> messageList;
    while(queryGetMessages.next())
    {
        messageStruct msg;
        msg.senderName = queryGetMessages.value(0).toString();
        msg.senderID = queryGetMessages.value(1).toInt();
        msg.text = queryGetMessages.value(2).toString();
        msg.time = queryGetMessages.value(3).toDateTime();
        messageList.append(msg);
    }

    return messageList;
}


void DataBaseConnector::addNewDialogMessage(unsigned int senderID, unsigned int reciverID, QString text, QDateTime time)
{
    QSqlQuery queryAddNewMessage;
    queryAddNewMessage.prepare("INSERT INTO Message (text, senderID, data) VALUES (:text, :senderID, :date)");
    queryAddNewMessage.bindValue(":text", text);
    queryAddNewMessage.bindValue(":senderID", senderID);
    queryAddNewMessage.bindValue(":date", time.toString("yyyy-MM-dd hh:mm:ss"));
    queryAddNewMessage.exec();

    ///need sync
    QSqlQuery queryGetID;
    queryGetID.prepare("SELECT last_insert_rowid()");
    queryGetID.exec();
    queryGetID.next();
    int idMsg = queryGetID.value(0).toInt();

    QSqlQuery queryAddDialogMessage;
    queryAddDialogMessage.prepare("INSERT INTO DialogMsg (MessageID, dialogID) VALUES "
                                  "(:id, (SELECT dialogID FROM Dialog WHERE "
                                  "(:idUser1 = userID1 AND :idUser2 = userID2) OR (:idUser2 = userID1 AND :idUser1 = userID2)))");
    queryAddDialogMessage.bindValue(":id", idMsg);
    queryAddDialogMessage.bindValue(":idUser1", senderID);
    queryAddDialogMessage.bindValue(":idUser2", reciverID);
    queryAddDialogMessage.exec();
}

QList<unsigned int> DataBaseConnector::addNewGroupMessage(unsigned int senderID, unsigned int groupID, QString text, QDateTime time)
{
    QSqlQuery queryAddNewMessage;
    queryAddNewMessage.prepare("INSERT INTO Message (text, senderID, data) VALUES (:text, :senderID, :date)");
    queryAddNewMessage.bindValue(":text", text);
    queryAddNewMessage.bindValue(":senderID", senderID);
    queryAddNewMessage.bindValue(":date", time.toString("yyyy-MM-dd hh:mm:ss"));
    queryAddNewMessage.exec();

    ///need sync
    QSqlQuery queryGetID;
    queryGetID.prepare("SELECT last_insert_rowid()");
    queryGetID.exec();
    queryGetID.next();
    int idMsg = queryGetID.value(0).toInt();

    QSqlQuery queryAddGroupMessage;
    queryAddGroupMessage.prepare("INSERT INTO GroupMsg (messageID, groupID) VALUES (:messageID, :groupID)");
    queryAddGroupMessage.bindValue(":messageID", idMsg);
    queryAddGroupMessage.bindValue(":groupID", groupID);
    queryAddGroupMessage.exec();

    QSqlQuery queryGetGroupUserList;
    queryGetGroupUserList.prepare("SELECT userID FROM GroupUser WHERE groupID = :groupID");
    queryGetGroupUserList.bindValue(":groupID", groupID);
    queryGetGroupUserList.exec();
    QList<unsigned int> list;
    while(queryGetGroupUserList.next())
    {
        unsigned int id = queryGetGroupUserList.value(0).toInt();
        list.append(id);
    }

    return list;
}

QList<unsigned int> DataBaseConnector::addNewChannelMessage(unsigned int channelID, QString text, QDateTime time)
{
    QSqlQuery queryAddNewMessage;
    queryAddNewMessage.prepare("INSERT INTO Message (text, senderID, data) VALUES (:text, :channelID, :date)");
    queryAddNewMessage.bindValue(":text", text);
    queryAddNewMessage.bindValue(":channelID", channelID);
    queryAddNewMessage.bindValue(":date", time.toString("yyyy-MM-dd hh:mm:ss"));
    queryAddNewMessage.exec();

    ///need sync
    QSqlQuery queryGetID;
    queryGetID.prepare("SELECT last_insert_rowid()");
    queryGetID.exec();
    queryGetID.next();
    int idMsg = queryGetID.value(0).toInt();

    QSqlQuery queryAddChannelMessage;
    queryAddChannelMessage.prepare("INSERT INTO ChannelMsg (messageID, channelID) VALUES (:messageID, :channelID)");
    queryAddChannelMessage.bindValue(":messageID", idMsg);
    queryAddChannelMessage.bindValue(":channelID", channelID);
    queryAddChannelMessage.exec();

    QSqlQuery queryGetChannelSubsList;
    queryGetChannelSubsList.prepare("SELECT userID FROM ChannelSubs WHERE channelID = :channelID");
    queryGetChannelSubsList.bindValue(":channelID", channelID);
    queryGetChannelSubsList.exec();
    QList<unsigned int> list;
    while(queryGetChannelSubsList.next())
    {
        unsigned int id = queryGetChannelSubsList.value(0).toInt();
        list.append(id);
    }

    return list;
}

advanUserInfo DataBaseConnector::getUserInfo(unsigned int id)
{
    QSqlQuery queryGetUserInfo;
    queryGetUserInfo.prepare("SELECT name, login, description, number FROM User WHERE id = :id");
    queryGetUserInfo.bindValue(":id", id);
    queryGetUserInfo.exec();
    queryGetUserInfo.next();

    advanUserInfo userInfo;
    userInfo.name = queryGetUserInfo.value(0).toString();
    userInfo.login = queryGetUserInfo.value(1).toString();
    userInfo.desc = queryGetUserInfo.value(2).toString();
    userInfo.number = queryGetUserInfo.value(3).toString();

    return userInfo;
}

advanChannelInfo DataBaseConnector::getChannelInfo(unsigned int id)
{
    QSqlQuery queryGetChannelInfo;
    queryGetChannelInfo.prepare("SELECT name, description, isPublic, publicLogin FROM Channel WHERE id = :id");
    queryGetChannelInfo.bindValue(":id", id);
    queryGetChannelInfo.exec();
    queryGetChannelInfo.next();

    QSqlQuery queryCountOfSubs;
    queryCountOfSubs.prepare("SELECT COUNT(*) FROM ChannelSubs WHERE channelID = :id");
    queryCountOfSubs.bindValue(":id", id);
    queryCountOfSubs.exec();
    queryCountOfSubs.next();

    advanChannelInfo channelInfo;
    channelInfo.name = queryGetChannelInfo.value(0).toString();
    channelInfo.desc = queryGetChannelInfo.value(1).toString();
    channelInfo.isPublic = queryGetChannelInfo.value(2).toBool();
    if(channelInfo.isPublic == true)
        channelInfo.publicLogin = queryGetChannelInfo.value(3).toString();
    channelInfo.countSubs = queryCountOfSubs.value(0).toInt();

    return channelInfo;
}

advanGroupInfo DataBaseConnector::getGroupInfo(unsigned int id)
{
    QSqlQuery queryGetGroupInfo;
    queryGetGroupInfo.prepare("SELECT name, description, isPublic, publicLogin FROM [Group] WHERE id = :id");
    queryGetGroupInfo.bindValue(":id", id);
    queryGetGroupInfo.exec();
    queryGetGroupInfo.next();

    QSqlQuery queryCountOfUsers;
    queryCountOfUsers.prepare("SELECT COUNT(*) FROM GroupUser WHERE groupID = :id");
    queryCountOfUsers.bindValue(":id", id);
    queryCountOfUsers.exec();
    queryCountOfUsers.next();

    QSqlQuery queryGetAdminInfo;
    queryGetAdminInfo.prepare("SELECT id, name FROM User WHERE id = (SELECT adminID FROM [Group] WHERE id = :id)");
    queryGetAdminInfo.bindValue(":id", id);
    queryGetAdminInfo.exec();
    queryGetAdminInfo.next();

    advanGroupInfo groupInfo;
    groupInfo.name = queryGetGroupInfo.value(0).toString();
    groupInfo.desc = queryGetGroupInfo.value(1).toString();
    groupInfo.isPublic = queryGetGroupInfo.value(2).toBool();
    if(groupInfo.isPublic == true)
        groupInfo.publicLogin = queryGetGroupInfo.value(3).toString();
    groupInfo.countUser = queryCountOfUsers.value(0).toInt();
    groupInfo.adminID = queryGetAdminInfo.value(0).toInt();
    groupInfo.adminName = queryGetAdminInfo.value(1).toString();

    return groupInfo;
}

QList<baseClientInfo> DataBaseConnector::getListOfGroupUsers(unsigned int groupID)
{
    QSqlQuery queryGetList;
    queryGetList.prepare("SELECT id, name FROM User WHERE id IN (SELECT userID FROM GroupUser WHERE groupID = :id)");
    queryGetList.bindValue(":id", groupID);
    queryGetList.exec();

    QList<baseClientInfo> list;
    while(queryGetList.next())
    {
        baseClientInfo info;
        info.type = 1;
        info.id = queryGetList.value(0).toInt();
        info.name = queryGetList.value(1).toString();
        list.append(info);
    }

    return list;

}

QList<baseClientInfo> DataBaseConnector::getListOfFriends(unsigned int id)
{
    QSqlQuery queryGetList;
    queryGetList.prepare("SELECT id, name FROM User WHERE id IN "
                         "(SELECT id2 FROM Friends WHERE id1 = :id union SELECT id1 FROM Friends WHERE id2 = :id)");
    queryGetList.bindValue(":id", id);
    queryGetList.exec();

    QList<baseClientInfo> list;
    while(queryGetList.next())
    {
        baseClientInfo info;
        info.type = 1;
        info.id = queryGetList.value(0).toInt();
        info.name = queryGetList.value(1).toString();
        list.append(info);
    }

    return list;
}

void DataBaseConnector::addNewUserInfo(unsigned int id, advanUserInfo info)
{
    QSqlQuery queryAddNewUserInfo;
    QString queryString = "UPDATE User SET ";
    bool isNotFirst = false;
    if(!info.name.isEmpty())
    {
        queryString += "name = :name";
        isNotFirst = true;
    }
    if(!info.desc.isEmpty())
    {
        if(isNotFirst)
            queryString += ", ";
        queryString += "description = :description";
        isNotFirst = true;
    }
    if(!info.number.isEmpty())
    {
        if(isNotFirst)
            queryString += ", ";
        queryString += "number = :number";
    }
    queryString += " WHERE id = :id";

    queryAddNewUserInfo.prepare(queryString);
    queryAddNewUserInfo.bindValue(":id", id);

    if(!info.name.isEmpty())
        queryAddNewUserInfo.bindValue(":name", info.name);
    if(!info.desc.isEmpty())
        queryAddNewUserInfo.bindValue(":description", info.desc);
    if(!info.number.isEmpty())
        queryAddNewUserInfo.bindValue(":number", info.number);

    queryAddNewUserInfo.exec();
}

QList<baseClientInfo> DataBaseConnector::getListOfChannelSubs(unsigned int id)
{
    QSqlQuery queryGetList;
    queryGetList.prepare("SELECT id, name FROM User WHERE id IN (SELECT userID FROM ChannelSubs WHERE channelID = :id) ");
    queryGetList.bindValue(":id", id);
    queryGetList.exec();

    QList<baseClientInfo> list;
    while(queryGetList.next())
    {
        baseClientInfo info;
        info.type = 1;
        info.id = queryGetList.value(0).toInt();
        info.name = queryGetList.value(1).toString();
        list.append(info);
    }

    return list;
}

void DataBaseConnector::deleteSubFromChannel(unsigned int userID, unsigned int channelID)
{
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM ChannelSubs WHERE userID = :userID AND channelID = :channelID");
    queryDelete.bindValue(":userID", userID);
    queryDelete.bindValue(":channelID", channelID);
    queryDelete.exec();

}

void DataBaseConnector::deleteUserFromGroup(unsigned int userID, unsigned int groupID)
{
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM GroupUser WHERE userID = :userID AND groupID = :groupID");
    queryDelete.bindValue(":userID", userID);
    queryDelete.bindValue(":groupID", groupID);
    queryDelete.exec();

}

void DataBaseConnector::addNewChannelInfo(unsigned int id, advanChannelInfo info)
{
    QSqlQuery queryAddNewChannelInfo;
    QString queryString = "UPDATE Channel SET ";
    bool isNotFirst = false;
    if(!info.name.isEmpty())
    {
        queryString += "name = :name";
        isNotFirst = true;
    }
    if(!info.desc.isEmpty())
    {
        if(isNotFirst)
            queryString += ", ";
        queryString += "description = :description";
        isNotFirst = true;
    }
    if(info.isPublic)
    {
        if(isNotFirst)
            queryString += ", ";
        queryString += "publicLogin = :publicLogin";
        isNotFirst = true;
    }
    if(isNotFirst)
        queryString += ", ";
    queryString += "isPublic = :isPublic";

    queryString += " WHERE id = :id";

    queryAddNewChannelInfo.prepare(queryString);
    queryAddNewChannelInfo.bindValue(":id", id);

    if(!info.name.isEmpty())
        queryAddNewChannelInfo.bindValue(":name", info.name);
    if(!info.desc.isEmpty())
        queryAddNewChannelInfo.bindValue(":description", info.desc);
    if(info.isPublic)
    {
        queryAddNewChannelInfo.bindValue(":publicLogin", info.publicLogin);
    }
    queryAddNewChannelInfo.bindValue(":isPublic", info.isPublic);

    queryAddNewChannelInfo.exec();
}

void DataBaseConnector::addNewGroupInfo(unsigned int id, advanGroupInfo info)
{
    QSqlQuery queryAddNewChannelInfo;
    QString queryString = "UPDATE [Group] SET ";
    bool isNotFirst = false;
    if(!info.name.isEmpty())
    {
        queryString += "name = :name";
        isNotFirst = true;
    }
    if(!info.desc.isEmpty())
    {
        if(isNotFirst)
            queryString += ", ";
        queryString += "description = :description";
        isNotFirst = true;
    }
    if(info.isPublic)
    {
        if(isNotFirst)
            queryString += ", ";
        queryString += "publicLogin = :publicLogin";
        isNotFirst = true;
    }
    if(isNotFirst)
        queryString += ", ";
    queryString += "isPublic = :isPublic";

    queryString += " WHERE id = :id";

    queryAddNewChannelInfo.prepare(queryString);
    queryAddNewChannelInfo.bindValue(":id", id);

    if(!info.name.isEmpty())
        queryAddNewChannelInfo.bindValue(":name", info.name);
    if(!info.desc.isEmpty())
        queryAddNewChannelInfo.bindValue(":description", info.desc);
    if(info.isPublic)
    {
        queryAddNewChannelInfo.bindValue(":publicLogin", info.publicLogin);
    }
    queryAddNewChannelInfo.bindValue(":isPublic", info.isPublic);


    queryAddNewChannelInfo.exec();
}

unsigned int DataBaseConnector::inviteUserToGroup(QString login, unsigned int groupID)
{
    QSqlQuery queryGetUserID;
    unsigned int id;
    queryGetUserID.prepare("SELECT id FROM User WHERE login = :login");
    queryGetUserID.bindValue(":login", login);
    queryGetUserID.exec();
    queryGetUserID.next();
    id = queryGetUserID.value(0).toInt();

    QSqlQuery queryInviteUserToGroup;
    queryInviteUserToGroup.prepare("INSERT INTO GroupUser (userID, groupID) VALUES (:id, :groupID)");
    queryInviteUserToGroup.bindValue(":login", login);
    queryInviteUserToGroup.bindValue(":id", id);
    queryInviteUserToGroup.bindValue(":groupID", groupID);
    queryInviteUserToGroup.exec();

    return id;
}



bool DataBaseConnector::isExistUser(QString login)
{
    QSqlQuery queryGetUser;
    queryGetUser.prepare("SELECT EXISTS(SELECT * FROM User WHERE login = :login)");
    queryGetUser.bindValue(":login", login);
    queryGetUser.exec();
    queryGetUser.next();
    int isExist = queryGetUser.value(0).toInt();
    return  isExist;
}

bool DataBaseConnector::isChannelLoginExistAlready(QString login)
{
    QSqlQuery queryGetChannel;
    queryGetChannel.prepare("SELECT EXISTS(SELECT * FROM Channel WHERE publicLogin = :publicLogin)");
    queryGetChannel.bindValue(":publicLogin", login);
    queryGetChannel.exec();
    queryGetChannel.next();
    int isExist = queryGetChannel.value(0).toInt();
    return  isExist;
}

unsigned int DataBaseConnector::createNewChannel(advanChannelInfo info, unsigned int adminID, QList<unsigned int > invitedUsers)
{
    QSqlQuery queryCreateNewChannel;
    queryCreateNewChannel.prepare("INSERT INTO Channel (name, description, isPublic, adminID, publicLogin)"
                                  " VALUES (:name, :desc, :isPublic, :adminID, :publicLogin)");
    queryCreateNewChannel.bindValue(":name", info.name);
    queryCreateNewChannel.bindValue(":desc", info.desc);
    queryCreateNewChannel.bindValue(":isPublic", info.isPublic);
    queryCreateNewChannel.bindValue(":adminID", adminID);
    queryCreateNewChannel.bindValue(":publicLogin", info.publicLogin);
    queryCreateNewChannel.exec();

    QSqlQuery queryGetID;
    queryGetID.prepare("SELECT last_insert_rowid()");
    queryGetID.exec();
    queryGetID.next();
    unsigned int channelID = queryGetID.value(0).toInt();

    QString query = "INSERT INTO ChannelSubs (userID, channelID) VALUES (:adminID, :channelID)";
    for(int i = 0; i < invitedUsers.count(); i++)
    {
        query += ", ";
        QString userID = "(:userID" + QString::number(i);
        query += userID;
        query += ", :channelID)";
    }

    QSqlQuery querySetUsers;
    querySetUsers.prepare(query);
    querySetUsers.bindValue(":adminID", adminID);
    for(int i = 0; i < invitedUsers.count(); i++)
    {
        QString userID = ":userID" + QString::number(i);
        querySetUsers.bindValue(userID, invitedUsers[i]);
    }
    querySetUsers.bindValue(":channelID", channelID);
    querySetUsers.exec();

    return channelID;
}

bool DataBaseConnector::isGroupLoginExistAlready(QString login)
{
    QSqlQuery queryGetChannel;
    queryGetChannel.prepare("SELECT EXISTS(SELECT * FROM [Group] WHERE publicLogin = :publicLogin)");
    queryGetChannel.bindValue(":publicLogin", login);
    queryGetChannel.exec();
    queryGetChannel.next();
    int isExist = queryGetChannel.value(0).toInt();
    return  isExist;
}

unsigned int DataBaseConnector::createNewGroup(advanGroupInfo info, unsigned int adminID, QList<unsigned int> invitedUsers)
{
    QSqlQuery queryCreateNewGroup;
    queryCreateNewGroup.prepare("INSERT INTO [Group] (name, description, isPublic, adminID, publicLogin)"
                                " VALUES (:name, :desc, :isPublic, :adminID, :publicLogin)");
    queryCreateNewGroup.bindValue(":name", info.name);
    queryCreateNewGroup.bindValue(":desc", info.desc);
    queryCreateNewGroup.bindValue(":isPublic", info.isPublic);
    queryCreateNewGroup.bindValue(":adminID", adminID);
    queryCreateNewGroup.bindValue(":publicLogin", info.publicLogin);
    queryCreateNewGroup.exec();

    QSqlQuery queryGetID;
    queryGetID.prepare("SELECT last_insert_rowid()");
    queryGetID.exec();
    queryGetID.next();
    unsigned int groupID = queryGetID.value(0).toInt();

    QString query = "INSERT INTO GroupUser (userID, groupID) VALUES (:adminID, :groupID)";
    for(int i = 0; i < invitedUsers.count(); i++)
    {
        query += ", ";
        QString userID = "(:userID" + QString::number(i);
        query += userID;
        query += ", :groupID)";
    }
    QSqlQuery querySetUsers;
    querySetUsers.prepare(query);
    querySetUsers.bindValue(":adminID", adminID);
    for(int i = 0; i < invitedUsers.count(); i++)
    {
        QString userID = ":userID" + QString::number(i);
        querySetUsers.bindValue(userID, invitedUsers[i]);
    }
    querySetUsers.bindValue(":groupID", groupID);
    querySetUsers.exec();

    return groupID;
}

void DataBaseConnector::deleteChannel(unsigned int id)
{
    QSqlQuery queryDeleteChannelSubs;
    queryDeleteChannelSubs.prepare("DELETE FROM ChannelSubs WHERE channelID = :channelID");
    queryDeleteChannelSubs.bindValue(":channelID", id);
    queryDeleteChannelSubs.exec();

    QSqlQuery queryDeleteChannelMessages;
    queryDeleteChannelMessages.prepare("DELETE FROM Message WHERE id IN (SELECT messageID FROM ChannelMsg WHERE :channelID = channelID)");
    queryDeleteChannelMessages.bindValue(":channelID", id);
    queryDeleteChannelMessages.exec();

    QSqlQuery queryDeleteChannel;
    queryDeleteChannel.prepare("DELETE FROM Channel WHERE id = :id");
    queryDeleteChannel.bindValue(":id", id);
    queryDeleteChannel.exec();
}

void DataBaseConnector::deleteGroup(unsigned int id)
{
    QSqlQuery queryDeleteGroupUsers;
    queryDeleteGroupUsers.prepare("DELETE FROM GroupUser WHERE groupID = :groupID");
    queryDeleteGroupUsers.bindValue(":groupID", id);
    queryDeleteGroupUsers.exec();

    QSqlQuery queryDeleteChannelMessages;
    queryDeleteChannelMessages.prepare("DELETE FROM Message WHERE id IN (SELECT messageID FROM GroupMsg WHERE :groupID = groupID)");
    queryDeleteChannelMessages.bindValue(":groupID", id);
    queryDeleteChannelMessages.exec();

    QSqlQuery queryDeleteGroup;
    queryDeleteGroup.prepare("DELETE FROM [Group] WHERE id = :id");
    queryDeleteGroup.bindValue(":id", id);
    queryDeleteGroup.exec();
}

QList<advanUserInfo> DataBaseConnector::searchUsers(QString login)
{
    QList<advanUserInfo> usersList;
    QSqlQuery querySearchUsers;
    querySearchUsers.prepare("SELECT id, login, name FROM User WHERE login LIKE :login || '%'");
    querySearchUsers.bindValue(":login", login);
    querySearchUsers.exec();

    while(querySearchUsers.next())
    {
        advanUserInfo user;
        user.id = querySearchUsers.value(0).toInt();
        user.login = querySearchUsers.value(1).toString();
        user.name = querySearchUsers.value(2).toString();
        usersList.append(user);
    }

    return usersList;
}

QList<advanChannelInfo> DataBaseConnector::searchChannels(QString login)
{
    QList<advanChannelInfo> channelsList;
    QSqlQuery querySearchUsers;
    querySearchUsers.prepare("SELECT id, publicLogin, name FROM Channel WHERE publicLogin LIKE :login || '%' AND isPublic = 1");
    querySearchUsers.bindValue(":login", login);
    querySearchUsers.exec();

    while(querySearchUsers.next())
    {
        advanChannelInfo channel;
        channel.id = querySearchUsers.value(0).toInt();
        channel.publicLogin = querySearchUsers.value(1).toString();
        channel.name = querySearchUsers.value(2).toString();
        channelsList.append(channel);
    }

    return channelsList;
}

QList<advanGroupInfo> DataBaseConnector::searchGroups(QString login)
{
    QList<advanGroupInfo> groupsList;
    QSqlQuery querySearchUsers;
    querySearchUsers.prepare("SELECT id, publicLogin, name FROM [Group] WHERE publicLogin LIKE :login || '%' AND isPublic = 1;");
    querySearchUsers.bindValue(":login", login);
    querySearchUsers.exec();

    while(querySearchUsers.next())
    {
        advanGroupInfo group;
        group.id = querySearchUsers.value(0).toInt();
        group.publicLogin = querySearchUsers.value(1).toString();
        group.name = querySearchUsers.value(2).toString();
        groupsList.append(group);
    }

    return groupsList;
}

void DataBaseConnector::startNewDialog(unsigned int id, unsigned int initUserID)
{
    QSqlQuery queryStartNewDialog;
    queryStartNewDialog.prepare("INSERT INTO Dialog (userID1, userID2) VALUES (:id1, :id2)");
    queryStartNewDialog.bindValue(":id1", initUserID);
    queryStartNewDialog.bindValue(":id2", id);
    queryStartNewDialog.exec();
}

void DataBaseConnector::joinToChannel(unsigned int channelID, unsigned int userID)
{
    QSqlQuery queryJoinToChannel;
    queryJoinToChannel.prepare("INSERT INTO ChannelSubs (channelID, userID) VALUES (:channelID, :userID)");
    queryJoinToChannel.bindValue(":channelID", channelID);
    queryJoinToChannel.bindValue(":userID", userID);
    queryJoinToChannel.exec();
}

void DataBaseConnector::joinToGroup(unsigned int groupID, unsigned int userID)
{
    QSqlQuery queryJoinToGroup;
    queryJoinToGroup.prepare("INSERT INTO GroupUser (groupID, userID) VALUES (:groupID, :userID)");
    queryJoinToGroup.bindValue(":groupID", groupID);
    queryJoinToGroup.bindValue(":userID", userID);
    queryJoinToGroup.exec();
}

unsigned int DataBaseConnector::deleteDialogMessage(unsigned int senderID, QDateTime date, QString text)
{
    QSqlQuery queryDeleteDialogMsg;
    queryDeleteDialogMsg.prepare("DELETE FROM DialogMsg WHERE messageID IN (SELECT id FROM Message WHERE :data = data AND :senderID = senderID)");
    queryDeleteDialogMsg.bindValue(":data", date.toString("yyyy-MM-dd hh:mm:ss"));
    queryDeleteDialogMsg.bindValue(":senderID", senderID);
    queryDeleteDialogMsg.exec();

    QSqlQuery queryDeleteMsg;
    queryDeleteMsg.prepare("DELETE FROM Message WHERE :data = data AND :senderID = senderID");
    queryDeleteMsg.bindValue(":data", date.toString("yyyy-MM-dd hh:mm:ss"));
    queryDeleteMsg.bindValue(":senderID", senderID);
    queryDeleteMsg.exec();
}

void DataBaseConnector::deleteChannelMessage(unsigned int senderID, QDateTime date, QString text)
{
    QSqlQuery queryDeleteChannelMsg;
    queryDeleteChannelMsg.prepare("DELETE FROM ChannelMsg WHERE messageID IN (SELECT id FROM Message WHERE :data = data AND :senderID = senderID)");
    queryDeleteChannelMsg.bindValue(":data", date.toString("yyyy-MM-dd hh:mm:ss"));
    queryDeleteChannelMsg.bindValue(":senderID", senderID);
    queryDeleteChannelMsg.exec();

    QSqlQuery queryDeleteMsg;
    queryDeleteMsg.prepare("DELETE FROM Message WHERE :data = data AND :senderID = senderID");
    queryDeleteMsg.bindValue(":data", date.toString("yyyy-MM-dd hh:mm:ss"));
    queryDeleteMsg.bindValue(":senderID", senderID);
    queryDeleteMsg.exec();
}

void DataBaseConnector::deleteGroupMessage(unsigned int senderID, QDateTime date, QString text)
{
    QSqlQuery queryDeleteGroupMsg;
    queryDeleteGroupMsg.prepare("DELETE FROM GroupMsg WHERE messageID IN (SELECT id FROM Message WHERE :data = data AND :senderID = senderID)");
    queryDeleteGroupMsg.bindValue(":data", date.toString("yyyy-MM-dd hh:mm:ss"));
    queryDeleteGroupMsg.bindValue(":senderID", senderID);
    queryDeleteGroupMsg.exec();

    QSqlQuery queryDeleteMsg;
    queryDeleteMsg.prepare("DELETE FROM Message WHERE :data = data AND :senderID = senderID");
    queryDeleteMsg.bindValue(":data", date.toString("yyyy-MM-dd hh:mm:ss"));
    queryDeleteMsg.bindValue(":senderID", senderID);
    queryDeleteMsg.exec();
}

void DataBaseConnector::editMessage(unsigned int senderID, QDateTime date, QString text, QString newText)
{
    QSqlQuery queryEditMessage;
    queryEditMessage.prepare("UPDATE Message SET text = :text WHERE :data = data AND :senderID = senderID");
    queryEditMessage.bindValue(":data", date.toString("yyyy-MM-dd hh:mm:ss"));
    queryEditMessage.bindValue(":senderID", senderID);
    queryEditMessage.bindValue(":text", newText);
    queryEditMessage.exec();
}

void DataBaseConnector::addFriend(unsigned int invitedID, unsigned int id)
{
    QSqlQuery queryAddFriend;
    queryAddFriend.prepare("INSERT INTO Friends (id1, id2) VALUES (:id1, :id2)");
    queryAddFriend.bindValue(":id1", invitedID);
    queryAddFriend.bindValue(":id2", id);
    queryAddFriend.exec();
}

void DataBaseConnector::removeFriend(unsigned int removedID, unsigned int id)
{
    QSqlQuery queryAddFriend;
    queryAddFriend.prepare("DELETE FROM Friends WHERE (id1 = :id1 AND id2 = :id2) OR (id2 = :id1 AND id1 = :id2)");
    queryAddFriend.bindValue(":id1", removedID);
    queryAddFriend.bindValue(":id2", id);
    queryAddFriend.exec();
}

unsigned int DataBaseConnector::inviteUserToChannel(QString login, unsigned int groupID)
{
    QSqlQuery queryGetUserID;
    unsigned int id;
    queryGetUserID.prepare("SELECT id FROM User WHERE login = :login");
    queryGetUserID.bindValue(":login", login);
    queryGetUserID.exec();
    queryGetUserID.next();
    id = queryGetUserID.value(0).toInt();

    QSqlQuery queryInviteUserToChannel;
    queryInviteUserToChannel.prepare("INSERT INTO ChannelSubs (userID, channelID) VALUES (:id, :channelID)");
    queryInviteUserToChannel.bindValue(":id", id);
    queryInviteUserToChannel.bindValue(":channelID", groupID);
    queryInviteUserToChannel.exec();

    return id;
}
