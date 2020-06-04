#include "client.h"

QDataStream &operator << (QDataStream &out, const baseClientInfo &list)
{
    out << list.type;
    out << list.id;
    out << list.name;
    return out;
}

QDataStream &operator << (QDataStream &out, const messageStruct &list)
{
    out << list.senderName;
    out << list.senderID;
    out << list.text;
    out << list.time;
    return out;
}

QDataStream &operator << (QDataStream &out, const advanUserInfo &list)
{
    out << list.id;
    out << list.name;
    out << list.login;
    out << list.desc;
    out << list.number;
    return out;
}

QDataStream &operator << (QDataStream &out, const advanChannelInfo &list)
{
    out << list.id;
    out << list.name;
    out << list.desc;
    out << list.isPublic;
    out << list.countSubs;
    out << list.publicLogin;
    return out;
}

QDataStream &operator << (QDataStream &out, const advanGroupInfo &list)
{
    out << list.id;
    out << list.name;
    out << list.desc;
    out << list.countUser;
    out << list.isPublic;
    out << list.adminID;
    out << list.adminName;
    out << list.publicLogin;
    return out;
}

QDataStream &operator >> (QDataStream &out, advanUserInfo &list)
{
    out >> list.id;
    out >> list.name;
    out >> list.login;
    out >> list.desc;
    out >> list.number;
    return out;
}

QDataStream &operator >> (QDataStream &out, advanChannelInfo &list)
{
    out >> list.id;
    out >> list.name;
    out >> list.desc;
    out >> list.isPublic;
    out >> list.countSubs;
    out >> list.publicLogin;
    return out;
}

QDataStream &operator >> (QDataStream &out, advanGroupInfo &list)
{
    out >> list.id;
    out >> list.name;
    out >> list.desc;
    out >> list.isPublic;
    out >> list.publicLogin;
    return out;
}

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

    emit disconnected(this);
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
                qDebug() << idClient;
                advanUserInfo authClientInfo = DataBaseConnector::getInstance()->getAuthClientInfo(idClient);

                emit loggined(idClient, this, authClientInfo);
            }
            else
            {
                qint8 comm = ServerCommand::errorAuth;
                sendCommandToClient(comm);
            }
            break;
        }

        case ServerCommand::clientLoadClientList:
        {
            int id;
            in >> id;
            qDebug() << id;
            QList<baseClientInfo> list;
            list = DataBaseConnector::getInstance()->getClientList(id);

            sendBaseClientInfo(id, list);
            break;

        }

        case ServerCommand::clientLoadDialogMessages:
        {
            unsigned int userID1;
            in >> userID1;
            unsigned int userID2;
            in >> userID2;
            QList<messageStruct> messageList;
            messageList = DataBaseConnector::getInstance()->getDialogMessages(userID1, userID2);
            sendDialogMessagesList(userID2, messageList);

            break;
        }

        case ServerCommand::clientLoadGroupMessages:
        {
            unsigned int groupID;
            in >> groupID;
            QList<messageStruct> messageList;
            messageList = DataBaseConnector::getInstance()->getGroupMessages(groupID);
            sendGroupMessagesList(groupID, messageList);

            break;
        }

        case ServerCommand::clientLoadChannelMessages:
        {
            unsigned int channelID;
            in >> channelID;
            QList<messageStruct> messageList;
            messageList = DataBaseConnector::getInstance()->getChannelMessages(channelID);
            sendChannelMessagesList(channelID, messageList);

            break;
        }

        case ServerCommand::clientSendMessageToUser:
        {
            QString senderName;
            in >> senderName;
            unsigned int senderID;
            in >> senderID;
            unsigned int reciverID;
            in >> reciverID;
            QString message;
            in >> message;
            QDateTime time;
            in >> time;
            DataBaseConnector::getInstance()->addNewDialogMessage(senderID, reciverID, message, time);

            messageStruct msg;
            msg.senderName = senderName;
            msg.senderID = senderID;
            msg.text = message;
            msg.time = time;

            sendDialogMessage(reciverID, msg);

            emit sendedDialogMessage(senderID, reciverID, msg);
            break;
        }

        case ServerCommand::clientSendGroupMessage:
        {
            QString senderName;
            in >> senderName;
            unsigned int senderID;
            in >> senderID;
            unsigned int groupID;
            in >> groupID;
            QString message;
            in >> message;
            QDateTime time;
            in >> time;

            QList<unsigned int> userList = DataBaseConnector::getInstance()->addNewGroupMessage(senderID, groupID, message, time);

            messageStruct msg;
            msg.senderName = senderName;
            msg.senderID = senderID;
            msg.text = message;
            msg.time = time;

            emit sendedGroupMessage(userList, groupID, msg);
            break;
        }

        case ServerCommand::clientSendChannelMessage:
        {
            QString channelName;
            in >> channelName;
            unsigned int channelID;
            in >> channelID;
            QString message;
            in >> message;
            QDateTime time;
            in >> time;

            QList<unsigned int> subsList = DataBaseConnector::getInstance()->addNewChannelMessage(channelID, message, time);

            messageStruct msg;
            msg.senderName = channelName;
            msg.senderID = channelID;
            msg.text = message;
            msg.time = time;

            emit sendedChannelMessage(subsList, channelID, msg);
            break;
        }

        case ServerCommand::clientLoadUserInfo:
        {
            unsigned int id;
            in >> id;
            advanUserInfo userinfo = DataBaseConnector::getInstance()->getUserInfo(id);

            sendUserInfo(id, userinfo);
            break;
        }

        case ServerCommand::clientLoadChannelInfo:
        {
            unsigned int id;
            in >> id;
            advanChannelInfo channelInfo = DataBaseConnector::getInstance()->getChannelInfo(id);

            sendChannelInfo(id, channelInfo);
            break;
        }

        case ServerCommand::clientLoadGroupInfo:
        {
            unsigned int id;
            in >> id;
            advanGroupInfo groupInfo = DataBaseConnector::getInstance()->getGroupInfo(id);
            QList<baseClientInfo> listOfGroupUsers = DataBaseConnector::getInstance()->getListOfGroupUsers(id);
            QList<advanUserInfo> usersInfo;
            for(int i = 0; i < listOfGroupUsers.count(); i++)
            {
                advanUserInfo info = DataBaseConnector::getInstance()->getUserInfo(listOfGroupUsers[i].id);
                usersInfo.append(info);
            }

            sendGroupInfo(id, groupInfo, listOfGroupUsers, usersInfo);
            break;
        }

        case ServerCommand::clientGetFriendsList:
        {
            unsigned int id;
            in >> id;
            QList<baseClientInfo> listOfFriends = DataBaseConnector::getInstance()->getListOfFriends(id);

            sendListOfFriends(listOfFriends);
            break;
        }

        case ServerCommand::clientSendNewUserInfo:
        {
            unsigned int id;
            in >> id;
            advanUserInfo info;
            in >> info;

            DataBaseConnector::getInstance()->addNewUserInfo(id, info);
            QList<baseClientInfo> listOfFriends = DataBaseConnector::getInstance()->getListOfFriends(id);

            advanUserInfo newInfo = DataBaseConnector::getInstance()->getUserInfo(id);

            QList<unsigned int> userList;
            for(int i = 0; i < listOfFriends.count(); i++)
            {
                userList.append(listOfFriends[i].id);
            }

            emit sendedNewUserInfo(userList, newInfo, id);
            break;
        }

        case ServerCommand::clientLoadChannelSubs:
        {
            unsigned int id;
            in >> id;

            QList<baseClientInfo> list = DataBaseConnector::getInstance()->getListOfChannelSubs(id);
            sendListOfChannelSubs(id, list);
            break;
        }

        case ServerCommand::clientLoadGroupUsers:
        {
            unsigned int id;
            in >> id;

            QList<baseClientInfo> list = DataBaseConnector::getInstance()->getListOfGroupUsers(id);
            sendListOfGroupUsers(id, list);
            break;
        }

        case ServerCommand::clientDeleteSubFromChannel:
        {
            unsigned int userID;
            in >> userID;
            unsigned int channelID;
            in >> channelID;

            QList<baseClientInfo> subList = DataBaseConnector::getInstance()->getListOfChannelSubs(channelID);
            DataBaseConnector::getInstance()->deleteSubFromChannel(userID, channelID);

            emit deletedSubFromChannel(subList, userID, channelID);
            break;
        }

        case ServerCommand::clientDeleteUserFromGroup:
        {
            unsigned int userID;
            in >> userID;
            unsigned int groupID;
            in >> groupID;

            QList<baseClientInfo> userList = DataBaseConnector::getInstance()->getListOfGroupUsers(groupID);
            DataBaseConnector::getInstance()->deleteUserFromGroup(userID, groupID);

            emit deletedUserFromGroup(userList, userID, groupID);
            break;
        }

        case ServerCommand::clientSendNewChannelInfo:
        {
            unsigned int channelID;
            in >> channelID;
            advanChannelInfo info;
            in >> info;

            if(info.isPublic)
            {
                if(DataBaseConnector::getInstance()->isChannelLoginExistAlready(info.publicLogin))
                {
                    sendCommandToClient(ServerCommand::errorLoginIsExistAlready);
                    return;
                }
            }

            DataBaseConnector::getInstance()->addNewChannelInfo(channelID, info);
            QList<baseClientInfo> subs = DataBaseConnector::getInstance()->getListOfChannelSubs(channelID);

            advanChannelInfo newInfo = DataBaseConnector::getInstance()->getChannelInfo(channelID);
            newInfo.id = channelID;

            sendNewChannelInfo(newInfo);

            emit sendedNewChannelInfo(subs, newInfo);
            break;
        }

        case ServerCommand::clientSendNewGroupInfo:
        {
            unsigned int groupID;
            in >> groupID;
            advanGroupInfo info;
            in >> info;

            if(info.isPublic)
            {
                if(DataBaseConnector::getInstance()->isGroupLoginExistAlready(info.publicLogin))
                {
                    sendCommandToClient(ServerCommand::errorLoginIsExistAlready);
                    return;
                }
            }

            DataBaseConnector::getInstance()->addNewGroupInfo(groupID, info);
            QList<baseClientInfo> userList = DataBaseConnector::getInstance()->getListOfGroupUsers(groupID);

            advanGroupInfo newInfo = DataBaseConnector::getInstance()->getGroupInfo(groupID);
            newInfo.id = groupID;

            emit sendedNewGroupInfo(userList, newInfo);

            break;
        }

        case ServerCommand::clientInviteUserToGroup:
        {
            QString login;
            in >> login;
            unsigned int groupID;
            in >> groupID;

            if(DataBaseConnector::getInstance()->isExistUser(login))
            {
                unsigned int userID;
                userID = DataBaseConnector::getInstance()->inviteUserToGroup(login, groupID);
                advanUserInfo userInfo;
                userInfo = DataBaseConnector::getInstance()->getUserInfo(userID);
                userInfo.id = userID;
                QList<baseClientInfo> list = DataBaseConnector::getInstance()->getListOfGroupUsers(groupID);
                advanGroupInfo groupInfo = DataBaseConnector::getInstance()->getGroupInfo(groupID);
                emit invitedUserToGroup(list, userInfo, groupID, groupInfo.name);

//                sendUserInfo(userID, userInfo);
            }
            else
            {
                sendCommandToClient(ServerCommand::errorUserIsNotExist);
            }


            break;
        }

        case ServerCommand::clientSendNewChannel:
        {
            advanChannelInfo info;
            in >> info;
            unsigned int adminID;
            in >> adminID;
            QList<unsigned int> invintedUsers;
            while(!in.atEnd())
            {
                unsigned int id;
                in >> id;
                invintedUsers.append(id);
            }

            if(info.isPublic)
            {
                if(DataBaseConnector::getInstance()->isChannelLoginExistAlready(info.publicLogin))
                {
                    sendCommandToClient(ServerCommand::errorLoginIsExistAlready);
                    return;
                }
            }

            unsigned int channelID = DataBaseConnector::getInstance()->createNewChannel(info, adminID, invintedUsers);
            advanChannelInfo channelInfo = DataBaseConnector::getInstance()->getChannelInfo(channelID);
            channelInfo.id = channelID;
            QList<baseClientInfo> list = DataBaseConnector::getInstance()->getListOfChannelSubs(channelID);
            sendNewChannel(channelInfo, list, adminID);
            emit invitedFriendsToNewChannel(invintedUsers, channelInfo, list, adminID);
            break;
        }

        case ServerCommand::clientSendNewGroup:
        {
            advanGroupInfo info;
            in >> info;
            unsigned int adminID;
            in >> adminID;
            QList<unsigned int> invintedUsers;
            while(!in.atEnd())
            {
                unsigned int id;
                in >> id;
                invintedUsers.append(id);
            }

            if(info.isPublic)
            {
                if(DataBaseConnector::getInstance()->isGroupLoginExistAlready(info.publicLogin))
                {
                    sendCommandToClient(ServerCommand::errorLoginIsExistAlready);
                    return;
                }
            }

            unsigned int groupID = DataBaseConnector::getInstance()->createNewGroup(info, adminID, invintedUsers);
            advanGroupInfo groupInfo = DataBaseConnector::getInstance()->getGroupInfo(groupID);
            groupInfo.id = groupID;
            QList<baseClientInfo> list = DataBaseConnector::getInstance()->getListOfGroupUsers(groupID);
            sendNewGroup(groupInfo, list);
            emit invitedFriendsToNewGroup(invintedUsers, groupInfo, list);
            break;
        }

        case ServerCommand::clientDeleteChannel:
        {
            unsigned int id;
            in >> id;

            QList<baseClientInfo> listOfUser = DataBaseConnector::getInstance()->getListOfChannelSubs(id);
            DataBaseConnector::getInstance()->deleteChannel(id);

            emit deletedChannel(id, listOfUser);
            break;
        }

        case ServerCommand::clientDeleteGroup:
        {
            unsigned int id;
            in >> id;

            QList<baseClientInfo> listOfUser = DataBaseConnector::getInstance()->getListOfGroupUsers(id);
            DataBaseConnector::getInstance()->deleteGroup(id);

            emit deletedGroup(id, listOfUser);
            break;
        }

        case ServerCommand::clientLeaveChannel:
        {
            unsigned int channelID;
            in >> channelID;
            unsigned int userID;
            in >> userID;

            DataBaseConnector::getInstance()->deleteSubFromChannel(userID, channelID);
            QList<baseClientInfo> list = DataBaseConnector::getInstance()->getListOfChannelSubs(channelID);
            emit leavedFromChannel(userID, channelID, list);
            break;
        }

        case ServerCommand::clientLeaveGroup:
        {
            unsigned int groupID;
            in >> groupID;
            unsigned int userID;
            in >> userID;

            DataBaseConnector::getInstance()->deleteUserFromGroup(userID, groupID);
            QList<baseClientInfo> list = DataBaseConnector::getInstance()->getListOfGroupUsers(groupID);
            emit leavedFromGroup(userID, groupID, list);
            break;
        }

        case ServerCommand::clientSearch:
        {
            QString login;
            in >> login;
            bool users, channels, groups;
            in >> users;
            in >> channels;
            in >> groups;

            QList<advanUserInfo> userList;
            QList<advanChannelInfo> channelList;
            QList<advanGroupInfo> groupList;

            if(users)
                userList = DataBaseConnector::getInstance()->searchUsers(login);
            if(channels)
                channelList = DataBaseConnector::getInstance()->searchChannels(login);
            if(groups)
                groupList = DataBaseConnector::getInstance()->searchGroups(login);

            sendSearch(userList, channelList, groupList);
            break;
        }

        case ServerCommand::clientStartNewDialog:
        {
            unsigned int id;
            in >> id;
            unsigned int initUserID;
            in >> initUserID;

            DataBaseConnector::getInstance()->startNewDialog(id, initUserID);
            advanUserInfo info = DataBaseConnector::getInstance()->getUserInfo(initUserID);
            info.id = initUserID;

            emit startedNewDialog(id, info);
            break;
        }

        case ServerCommand::clientJoinToChannel:
        {
            unsigned int channelID;
            in >> channelID;
            unsigned int userID;
            in >> userID;

            DataBaseConnector::getInstance()->joinToChannel(channelID, userID);
            break;
        }

        case ServerCommand::clientJoinToGroup:
        {
            unsigned int groupID;
            in >> groupID;
            unsigned int userID;
            in >> userID;

            DataBaseConnector::getInstance()->joinToGroup(groupID, userID);
            break;
        }

        case ServerCommand::clientDeleteMessage:
        {
            unsigned int senderID;
            in >> senderID;
            int type;
            in >> type;
            QDateTime date;
            in >> date;
            QString text;
            in >> text;
            unsigned int id;
            in >> id;

            QList<baseClientInfo> list;
            switch (type)
            {
            case 1:
            {
                DataBaseConnector::getInstance()->deleteDialogMessage(senderID, date, text);
                baseClientInfo info;
                info.type = 1;
                info.id = id;
                list.append(info);
                break;
            }
            case 2:
            {
                DataBaseConnector::getInstance()->deleteChannelMessage(senderID, date, text);
                list = DataBaseConnector::getInstance()->getListOfChannelSubs(id);
                break;
            }
            case 3:
            {
                DataBaseConnector::getInstance()->deleteGroupMessage(senderID, date, text);
                list = DataBaseConnector::getInstance()->getListOfGroupUsers(id);
                break;
            }
            }

            emit deletedMessage(senderID, date, type, list, id);

            break;
        }

        case ServerCommand::clientEditMessage:
        {
            unsigned int senderID;
            in >> senderID;
            QDateTime date;
            in >> date;
            QString text;
            in >> text;
            QString newText;
            in >> newText;
            int type;
            in >> type;
            unsigned int id;
            in >> id;

            DataBaseConnector::getInstance()->editMessage(senderID, date, text, newText);

            QList<baseClientInfo> list;
            switch (type)
            {
            case 1:
            {
                baseClientInfo info;
                info.type = 1;
                info.id = id;
                list.append(info);
                break;
            }
            case 2:
            {
                list = DataBaseConnector::getInstance()->getListOfChannelSubs(id);
                break;
            }
            case 3:
            {
                list = DataBaseConnector::getInstance()->getListOfGroupUsers(id);
                break;
            }
            }

            emit changedMessage(senderID, newText, date, type, list, id);
            break;
        }

        case ServerCommand::clientAddToFriendsList:
        {
            unsigned int invitedID;
            in >> invitedID;
            unsigned int id;
            in >> id;

            DataBaseConnector::getInstance()->addFriend(invitedID, id);

            advanUserInfo info = DataBaseConnector::getInstance()->getUserInfo(id);
            info.id = id;

            emit addedToFriendsList(invitedID, info);
            break;
        }

        case ServerCommand::clientDeleteFriend:
        {
            unsigned int removedID;
            in >> removedID;
            unsigned int id;
            in >> id;

            DataBaseConnector::getInstance()->removeFriend(removedID, id);
            emit deletedFriend(removedID, id);
            break;
        }

        case ServerCommand::clientInviteUserToChannel:
        {
            QString login;
            in >> login;
            unsigned int channelID;
            in >> channelID;

            if(DataBaseConnector::getInstance()->isExistUser(login))
            {
                unsigned int userID;
                userID = DataBaseConnector::getInstance()->inviteUserToChannel(login, channelID);
                advanUserInfo userInfo;
                userInfo = DataBaseConnector::getInstance()->getUserInfo(userID);
                userInfo.id = userID;
                QList<baseClientInfo> list = DataBaseConnector::getInstance()->getListOfChannelSubs(channelID);
                advanChannelInfo channelInfo = DataBaseConnector::getInstance()->getChannelInfo(channelID);
                emit invitedUserToChannel(list, userInfo, channelID, channelInfo.name);

            }
            else
            {
                sendCommandToClient(ServerCommand::errorUserIsNotExist);
            }
        }
    }
}

void Client::sendCommandToClient(qint8 comm)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << comm;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendAuthClientInfo(int id, advanUserInfo info)
{
    qDebug() << id;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servAuthSuccess;
    out << id;
    //need fix
    out << info;

    QList<unsigned int> channelAdmins = DataBaseConnector::getInstance()->getAdminsChannel(id);
    QList<unsigned int> groupAdmins = DataBaseConnector::getInstance()->getAdminsGroup(id);

    out << channelAdmins.count();
    for(int i = 0; i < channelAdmins.count(); i++)
    {
        out << channelAdmins[i];
    }

    out << groupAdmins.count();
    for(int i = 0; i < groupAdmins.count(); i++)
    {
        out << groupAdmins[i];
    }

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendBaseClientInfo(unsigned int userID, QList<baseClientInfo> list)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servLoadedClientList;
    for (int i = 0; i < list.count(); i++) {
        out << list[i];
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendDialogMessagesList(unsigned int userID, QList<messageStruct> list)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servLoadedDialogMessages;
    out << userID;
    for (int i = 0; i < list.count(); i++) {
        out << list[i];
        qDebug() << "CHANNEL mSGS" << list[i].text;

    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendGroupMessagesList(unsigned int groupID, QList<messageStruct> list)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servLoadedGroupMessages;
    out << groupID;
    for (int i = 0; i < list.count(); i++) {
        out << list[i];
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendChannelMessagesList(unsigned int channelID, QList<messageStruct> list)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servLoadedChannelMessages;
    out << channelID;
    for (int i = 0; i < list.count(); i++) {
        out << list[i];
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendDialogMessage(unsigned int reciverID, messageStruct msg)
{
    qDebug() << "SEND DIALOG MESSAGE";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servSendDialogMessage;
    out << reciverID;
    out << msg;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendGroupMessage(unsigned int groupID, messageStruct msg)
{
    qDebug() << "SEND GROUP MESSAGE";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servSendGroupMessage;
    out << groupID;
    out << msg;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendChannelMessage(unsigned int channelID, messageStruct msg)
{
    qDebug() << "SEND GROUP MESSAGE";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servSendChannelMessage;
    out << channelID;
    out << msg;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendUserInfo(unsigned int id, advanUserInfo userInfo)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servLoadUserInfo;
    out << id;
    out << userInfo;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendChannelInfo(unsigned int id, advanChannelInfo channelInfo)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servLoadChannelInfo;
    out << id;
    out << channelInfo;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendGroupInfo(unsigned int id, advanGroupInfo groupInfo, QList<baseClientInfo> listOfUsers, QList<advanUserInfo> usersiInfo)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servLoadGroupInfo;
    out << id;
    out << groupInfo;
    for (int i = 0; i < listOfUsers.count(); i++)
    {
        out << listOfUsers[i];
        out << usersiInfo[i];
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendListOfFriends(QList<baseClientInfo> list)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servGetFriendsList;
    for (int i = 0; i < list.count(); i++)
    {
        out << list[i];
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendListOfChannelSubs(unsigned int channelID, QList<baseClientInfo> list)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servLoadChannelSubs;
    out << channelID;
    for (int i = 0; i < list.count(); i++)
    {
        out << list[i];
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendListOfGroupUsers(unsigned int groupID, QList<baseClientInfo> list)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servLoadGroupUsers;
    out << groupID;
    for (int i = 0; i < list.count(); i++)
    {
        out << list[i];
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendNewChannel(advanChannelInfo info, QList<baseClientInfo> listOfUsers, unsigned int adminID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servSendNewChannel;
    out << info;
    out << adminID;
    for(int i = 0; i < listOfUsers.count(); i++)
    {
        out << listOfUsers[i];
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendNewGroup(advanGroupInfo info, QList<baseClientInfo> list)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servSendNewGroup;
    out << info;
    for(int i = 0; i < list.count(); i++)
    {
        out << list[i];
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendSearch(QList<advanUserInfo> userList, QList<advanChannelInfo> channelList, QList<advanGroupInfo> groupList)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servSearch;
    if(!userList.isEmpty())
    {
        out << userList.count();
        for(int i = 0; i < userList.count(); i++)
        {
            out << userList[i];
        }
    }
    else
        out << 0;

    if(!channelList.isEmpty())
    {
        out << channelList.count();
        for(int i = 0; i < channelList.count(); i++)
        {
            out << channelList[i];
        }
    }
    else
        out << 0;

    if(!groupList.isEmpty())
    {
        out << groupList.count();
        for(int i = 0; i < groupList.count(); i++)
        {
            out << groupList[i];
        }
    }
    else
        out << 0;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendNewUserInfo(unsigned int id, advanUserInfo userInfo)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servSendNewUserInfo;
    out << id;
    out << userInfo;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendNewChannelInfo(advanChannelInfo channelInfo)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servSendNewChannelInfo;
    out << channelInfo;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendNewGroupInfo(advanGroupInfo info)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servSendNewGroupInfo;
    out << info;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::deleteSubFromChannel(unsigned int userID, unsigned int channelID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servDeleteSubFromChannel;
    out << userID;
    out << channelID;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::deleteUserFromGroup(unsigned int userID, unsigned int groupID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servDeleteUserFromGroup;
    out << userID;
    out << groupID;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendInvitedUserInfo(advanUserInfo info, unsigned int groupID, QString groupName)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servInviteUserToGroup;
    out << info;
    out << groupID;
    out << groupName;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendInvitedUserInfoChannel(advanUserInfo info, unsigned int channelID, QString channelName)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servInviteUserToChannel;
    out << info;
    out << channelID;
    out << channelName;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendDeletedGroup(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servDeleteGroup;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendDeletedChannel(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servDeleteChannel;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendLeavedFromGroup(unsigned int userID, unsigned int groupID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servLeaveGroup;
    out << userID;
    out << groupID;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendLeavedFromChannel(unsigned int userID, unsigned int channelID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servLeaveChannel;
    out << userID;
    out << channelID;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendDeletedMessage(unsigned int senderID, QDateTime date, int type, unsigned int groupID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servDeleteMessage;
    out << senderID;
    out << date;
    out << type;
    if(type == 3)
    {
        out << groupID;
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendChangedMessage(unsigned int senderID,  QString newText, QDateTime date, int type, unsigned int groupID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servEditMessage;
    out << senderID;
    out << newText;
    out << date;
    out << type;
    if(type == 3)
    {
        out << groupID;
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendNewDialog(advanUserInfo info)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servStartNewDialog;
    out << info;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendAddedToFriendsList(advanUserInfo info)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servAddToFriendsList;
    out << info;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void Client::sendDeletedFriend(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << ServerCommand::servDeleteFriend;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

Client::~Client()
{

}
