#include "connector.h"
#include <QDebug>
#include <QThread>
#include "qdatastream.h"

QDataStream &operator >>(QDataStream &out, baseClientInfo &info)
{
    out >> info.type;
    out >> info.id;
    out >> info.name;
    return out;
}

QDataStream &operator >>(QDataStream &out, messageStruct &msg)
{
    out >> msg.senderName;
    out >> msg.senderID;
    out >> msg.text;
    out >> msg.time;
    return out;
}

QDataStream &operator >>(QDataStream &out, advanUserInfo &info)
{
	out >> info.id;
    out >> info.name;
    out >> info.login;
    out >> info.desc;
    out >> info.number;
    return out;
}

QDataStream &operator >>(QDataStream &out, advanChannelInfo &info)
{
	out >> info.id;
    out >> info.name;
    out >> info.desc;
    out >> info.isPublic;
    out >> info.countSubs;
	out >> info.publicLogin;
    return out;
}

QDataStream &operator >>(QDataStream &out, advanGroupInfo &info)
{
	out >> info.id;
    out >> info.name;
    out >> info.desc;
    out >> info.countUser;
    out >> info.isPublic;
    out >> info.adminID;
    out >> info.adminName;
	out >> info.publicLogin;
    return out;
}

QDataStream &operator <<(QDataStream &out, advanUserInfo &info)
{
	out << info.id;
    out << info.name;
    out << info.login;
    out << info.desc;
    out << info.number;
    return out;
}

QDataStream &operator <<(QDataStream &out, advanChannelInfo &info)
{
	out << info.id;
    out << info.name;
    out << info.desc;
    out << info.isPublic;
    out << info.countSubs;
	out << info.publicLogin;
    return out;
}

QDataStream &operator <<(QDataStream &out, advanGroupInfo &info)
{
	out << info.id;
    out << info.name;
    out << info.desc;
    out << info.isPublic;
	out << info.publicLogin;
    return out;
}

Connector* Connector::instance = 0;

Connector::Connector()
{

}

Connector* Connector::getInstance()
{
    if(instance == 0)
    {
        instance = new Connector();
        instance->_socket = new QTcpSocket(instance);
        instance->connectToServer();
        instance->_blockSize = 0;
    }

    return instance;
}

void Connector::connectToServer()
{
    _socket->connectToHost(QHostAddress("192.168.1.3"), 1234);

//    connect(_socket, &QTcpSocket::connected, this, SLOT(onSokConnected()));
//    connect(_socket, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(_socket, &QTcpSocket::readyRead, this, &Connector::onSokReadyRead);
}

void Connector::onSokReadyRead()
{
	qDebug() << 123;
	QDataStream in(_socket);
	if (_blockSize == 0) {
		if (_socket->bytesAvailable() < (int)sizeof(quint16))
		{
			return;
		}
		in >> _blockSize;
		qDebug() << "_blockSize now " << _blockSize;
	}
	if (_socket->bytesAvailable() < _blockSize)
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
	case ServerCommand::servRegSuccess:
	{
		emit registered();
		break;
	}
	case ServerCommand::servAuthSuccess:
	{
		///errorException need;
		unsigned int id;
		in >> id;
		advanUserInfo info;
		in >> info;
		_userInfo = new UserInfo(id, info.name, info.login, info.desc, info.number);

		int countChannelAdmins;
		in >> countChannelAdmins;
		for (int i = 0; i < countChannelAdmins; i++)
		{
			unsigned int id;
			in >> id;
			_listOfAdminChannel.append(id);
			_listOfChannel.append(id);
		}

		int countGroupAdmins;
		in >> countGroupAdmins;
		for (int i = 0; i < countGroupAdmins; i++)
		{
			unsigned int id;
			in >> id;
			_listOfAdminGroup.append(id);
			_listOfGroup.append(id);
		}

		logined = true;
		//emit loggedIn();
		break;
	}
	case ServerCommand::servLoadedClientList:
	{
		while (!in.atEnd())
		{
			baseClientInfo info;
			in >> info;
			Client* client;
			switch (info.type)
			{
			case 1:
			{

				UserInfo* pInfo = new UserInfo(info.id, info.name);
				client = new User(pInfo);
				_listOfDialogsUsers.append(info.id);
				break;
			}
			case 2:
			{
				ChannelInfo* pInfo = new ChannelInfo(info.id, info.name);
				client = new Channel(pInfo);
				_listOfChannel.append(info.id);
				break;
			}
			case 3:
			{
				GroupInfo* pInfo = new GroupInfo(info.id, info.name);
				client = new Group(pInfo);
				_listOfGroup.append(info.id);
				break;
			}
			}
			_listOfClient.append(client);
		}

		emit loadedClientList();
		break;
	}

	case ServerCommand::servLoadedDialogMessages:
	{
		unsigned int userID;
		in >> userID;
		Chat *chat = new Chat();
		while (!in.atEnd())
		{
			qDebug() << "servMSG";
			messageStruct msgStruct;
			in >> msgStruct;
			Message *msg = new Message(msgStruct.senderID, msgStruct.senderName, msgStruct.text, msgStruct.time);
			chat->addMessage(msg);
		}

		User* user = getUser(userID);
		user->setChat(chat);
		emit loadedDialogChat(userID);
		break;
	}

	case ServerCommand::servLoadedGroupMessages:
	{
		unsigned int groupID;
		in >> groupID;
		Chat *chat = new Chat();
		while (!in.atEnd())
		{
			qDebug() << "servMSG";
			messageStruct msgStruct;
			in >> msgStruct;
			Message *msg = new Message(msgStruct.senderID, msgStruct.senderName, msgStruct.text, msgStruct.time);
			chat->addMessage(msg);
		}

		Group* group = getGroup(groupID);
		group->setChat(chat);
		emit loadedGroupChat(groupID);
		break;
	}

	case ServerCommand::servLoadedChannelMessages:
	{
		unsigned int channelID;
		in >> channelID;
		Chat *chat = new Chat();
		while (!in.atEnd())
		{
			qDebug() << "servMSG";
			messageStruct msgStruct;
			in >> msgStruct;
			Message *msg = new Message(msgStruct.senderID, msgStruct.senderName, msgStruct.text, msgStruct.time);
			chat->addMessage(msg);
		}

		Channel* channel = getChannel(channelID);
		channel->setChat(chat);
		emit loadedChannelChat(channelID);

		break;
	}

	case ServerCommand::servSendDialogMessage:
	{
		qDebug() << "servSendDialogMessage";
		unsigned int id;
		in >> id;
		messageStruct msgStruct;
		in >> msgStruct;
		Message *msg = new Message(msgStruct.senderID, msgStruct.senderName, msgStruct.text, msgStruct.time);

		if (getUser(id)->getChat() != NULL)
		{
			getUser(id)->getChat()->addMessage(msg);
		}
		emit loadedNewDialogMessage(id);

		break;
	}

	case ServerCommand::servSendGroupMessage:
	{
		qDebug() << "servSendGroupMessage";
		unsigned int groupID;
		in >> groupID;
		messageStruct msgStruct;
		in >> msgStruct;
		Message *msg = new Message(msgStruct.senderID, msgStruct.senderName, msgStruct.text, msgStruct.time);

		if (getGroup(groupID)->getChat() != NULL)
		{
			getGroup(groupID)->getChat()->addMessage(msg);
		}
		emit loadedNewGroupMessage(groupID);

		break;
	}

	case ServerCommand::servSendChannelMessage:
	{
		qDebug() << "servSendChannelMessage";
		unsigned int channelID;
		in >> channelID;
		messageStruct msgStruct;
		in >> msgStruct;
		Message *msg = new Message(msgStruct.senderID, msgStruct.senderName, msgStruct.text, msgStruct.time);

		if (getChannel(channelID)->getChat() != NULL)
		{
			getChannel(channelID)->getChat()->addMessage(msg);
		}
		emit loadedNewChannelMessage(channelID);

		break;
	}

	case ServerCommand::servLoadUserInfo:
	{
		unsigned int id;
		in >> id;
		advanUserInfo userInfoStruct;
		in >> userInfoStruct;
		if (getUser(id) == NULL)
		{
			UserInfo *userInfo = new UserInfo(id);
			userInfo->setName(userInfoStruct.name);
			userInfo->setLogin(userInfoStruct.login);
			userInfo->setDesc(userInfoStruct.desc);
			userInfo->setNubmer(userInfoStruct.number);
			User *user = new User(userInfo);
			_listOfClient.append(user);
		}
		else
		{
			UserInfo *userInfo = dynamic_cast<UserInfo*>(getUser(id)->getInfo());
			userInfo->setName(userInfoStruct.name);
			userInfo->setLogin(userInfoStruct.login);
			userInfo->setDesc(userInfoStruct.desc);
			userInfo->setNubmer(userInfoStruct.number);
		}

		break;
	}

	case ServerCommand::servLoadChannelInfo:
	{
		unsigned int id;
		in >> id;
		advanChannelInfo channelInfoStruct;
		in >> channelInfoStruct;
		ChannelInfo *channelInfo = dynamic_cast<ChannelInfo*>(getChannel(id)->getInfo());
		channelInfo->setDesc(channelInfoStruct.desc);
		channelInfo->setCountOfSubs(channelInfoStruct.countSubs);
		channelInfo->setPublic(channelInfoStruct.isPublic);
		if (channelInfoStruct.isPublic)
			channelInfo->setPublicLogin(channelInfoStruct.publicLogin);

		break;
	}

	case ServerCommand::servLoadGroupInfo:
	{
		unsigned int id;
		in >> id;
		advanGroupInfo groupInfoStruct;
		in >> groupInfoStruct;
		GroupInfo *groupInfo = dynamic_cast<GroupInfo*>(getGroup(id)->getInfo());
		groupInfo->setName(groupInfoStruct.name);
		groupInfo->setDesc(groupInfoStruct.desc);
		groupInfo->setPublic(groupInfoStruct.isPublic);
		groupInfo->setCountOfUsers(groupInfoStruct.countUser);
		groupInfo->setAdminID(groupInfoStruct.adminID);
		groupInfo->setAdminName(groupInfoStruct.adminName);
		groupInfo->setPublic(groupInfoStruct.isPublic);
		if (groupInfoStruct.isPublic)
			groupInfo->setPublicLogin(groupInfoStruct.publicLogin);

		while (!in.atEnd())
		{
			baseClientInfo userInfoStruct;
			in >> userInfoStruct;
			UserInfo *userInfo;
			if (getUser(userInfoStruct.id) == NULL)
			{
				userInfo = new UserInfo(userInfoStruct.id, userInfoStruct.name);
				advanUserInfo info;
				in >> info;
				userInfo->setLogin(info.login);
				userInfo->setDesc(info.desc);
				userInfo->setNubmer(info.number);

				User *user = new User(userInfo);
				_listOfClient.append(user);
			}
			else
			{
				userInfo = dynamic_cast<UserInfo*>(getUser(userInfoStruct.id)->getInfo());
				advanUserInfo info;
				in >> info;
				userInfo->setLogin(info.login);
				userInfo->setDesc(info.desc);
				userInfo->setNubmer(info.number);
			}

			if (!groupInfo->getListOfUser().contains(dynamic_cast<UserInfo*>(getUser(userInfoStruct.id)->getInfo())))
			{
				groupInfo->addNewUser(dynamic_cast<UserInfo*>(getUser(userInfoStruct.id)->getInfo()));
			}
		}

		break;
	}

	case ServerCommand::servGetFriendsList:
	{
		while (!in.atEnd())
		{
			baseClientInfo userInfoStruct;
			in >> userInfoStruct;
			if (getUser(userInfoStruct.id) == NULL)
			{
				UserInfo *userInfo = new UserInfo(userInfoStruct.id, userInfoStruct.name);
				User *user = new User(userInfo);
				_listOfClient.append(user);
				_listOfFriends.append(user);
			}
			else
			{
				if (!_listOfFriends.contains(getUser(userInfoStruct.id)))
				{
					_listOfFriends.append(getUser(userInfoStruct.id));
				}
			}
		}

		break;
	}

	case ServerCommand::servLoadChannelSubs:
	{
		unsigned int channelID;
		in >> channelID;
		while (!in.atEnd())
		{
			baseClientInfo userInfoStruct;
			in >> userInfoStruct;
			UserInfo *userInfo;
			if (getUser(userInfoStruct.id) == NULL)
			{
				userInfo = new UserInfo(userInfoStruct.id, userInfoStruct.name);
				User *user = new User(userInfo);
				_listOfClient.append(user);
			}
			else
			{
				userInfo = dynamic_cast<UserInfo*>(getUser(userInfoStruct.id)->getInfo());
			}
			if (!dynamic_cast<ChannelInfo*>(getChannel(channelID)->getInfo())->getListOfUsers().contains(userInfo))
			{
				dynamic_cast<ChannelInfo*>(getChannel(channelID)->getInfo())->addNewUser(userInfo);
			}
		}

		break;
	}

	case ServerCommand::servLoadGroupUsers:
	{
		unsigned int groupID;
		in >> groupID;
		while (!in.atEnd())
		{
			baseClientInfo userInfoStruct;
			in >> userInfoStruct;
			UserInfo *userInfo;
			if (getUser(userInfoStruct.id) == NULL)
			{
				userInfo = new UserInfo(userInfoStruct.id, userInfoStruct.name);
				User *user = new User(userInfo);
				_listOfClient.append(user);
			}
			else
			{
				userInfo = dynamic_cast<UserInfo*>(getUser(userInfoStruct.id)->getInfo());
			}
			if (!dynamic_cast<GroupInfo*>(getGroup(groupID)->getInfo())->getListOfUser().contains(userInfo))
			{
				dynamic_cast<GroupInfo*>(getGroup(groupID)->getInfo())->addNewUser(userInfo);
			}
		}
		break;
	}

	case ServerCommand::servSendNewChannel:
	{
		advanChannelInfo info;
		in >> info;
		unsigned int adminID;
		in >> adminID;
		ChannelInfo *channelInfo = new ChannelInfo(info.id);
		Channel *channel = new Channel(channelInfo);
		_listOfClient.append(channel);

		if (adminID == _userInfo->getID())
		{
			_listOfAdminChannel.append(info.id);
			_listOfChannel.append(info.id);
			while (!in.atEnd())
			{
				baseClientInfo userInfoStruct;
				in >> userInfoStruct;
			}
		}
		else
		{
			while (!in.atEnd())
			{
				baseClientInfo userInfoStruct;
				in >> userInfoStruct;
				UserInfo *userInfo;
				if (getUser(userInfoStruct.id) == NULL)
				{
					userInfo = new UserInfo(userInfoStruct.id, userInfoStruct.name);

					User *user = new User(userInfo);
					_listOfClient.append(user);
				}
				else
				{
					userInfo = dynamic_cast<UserInfo*>(getUser(userInfoStruct.id)->getInfo());
				}

				if (!channelInfo->getListOfUsers().contains(dynamic_cast<UserInfo*>(getUser(userInfoStruct.id)->getInfo())))
				{
					channelInfo->addNewUser(dynamic_cast<UserInfo*>(getUser(userInfoStruct.id)->getInfo()));
				}
			}

			_listOfChannel.append(info.id);
			channelInfo->setName(info.name);
			emit addedByClient(info.id, channelInfo);
		}

		break;
	}

	case ServerCommand::servSendNewGroup:
	{
		advanGroupInfo info;
		in >> info;

		GroupInfo *groupInfo = new GroupInfo(info.id);
		Group *group = new Group(groupInfo);
		_listOfClient.append(group);
		newGroupID = info.id;

		if (info.adminID == _userInfo->getID())
		{
			_listOfAdminGroup.append(info.id);
			_listOfGroup.append(info.id);
			while (!in.atEnd())
			{
				baseClientInfo userInfoStruct;
				in >> userInfoStruct;
			}
		}
		else
		{
			while (!in.atEnd())
			{
				baseClientInfo userInfoStruct;
				in >> userInfoStruct;
				UserInfo *userInfo;
				if (getUser(userInfoStruct.id) == NULL)
				{
					userInfo = new UserInfo(userInfoStruct.id, userInfoStruct.name);

					User *user = new User(userInfo);
					_listOfClient.append(user);
				}
				else
				{
					userInfo = dynamic_cast<UserInfo*>(getUser(userInfoStruct.id)->getInfo());
				}

				if (!groupInfo->getListOfUser().contains(dynamic_cast<UserInfo*>(getUser(userInfoStruct.id)->getInfo())))
				{
					groupInfo->addNewUser(dynamic_cast<UserInfo*>(getUser(userInfoStruct.id)->getInfo()));
				}
			}

			_listOfGroup.append(info.id);
			groupInfo->setName(info.name);
			emit addedByClient(info.id, groupInfo);
		}

		break;
	}

	case ServerCommand::servSearch:
	{
		int users, channels, groups;
		in >> users;
		_foundClients.clear();
		for (int i = 0; i < users; i++)
		{
			advanUserInfo info;
			in >> info;
			if (getUser(info.id) == NULL)
			{
				UserInfo *userInfo = new UserInfo(info.id, info.name, info.login);
				User *user = new User(userInfo);
				_listOfClient.append(user);
				_foundClients.append(user);
			}
			else
			{
				UserInfo *userInfo = dynamic_cast<UserInfo*>(getUser(info.id)->getInfo());
				userInfo->setName(info.name);
				userInfo->setLogin(info.login);
				_foundClients.append(getUser(info.id));
			}
			int s = 1;
		}

		in >> channels;
		for (int i = 0; i < channels; i++)
		{
			advanChannelInfo info;
			in >> info;
			if (getChannel(info.id) == NULL)
			{
				ChannelInfo *channelInfo = new ChannelInfo(info.id, info.name);
				channelInfo->setPublicLogin(info.publicLogin);
				Channel *channel = new Channel(channelInfo);
				_listOfClient.append(channel);
				_foundClients.append(channel);
			}
			else
			{
				ChannelInfo *channelInfo = dynamic_cast<ChannelInfo*>(getChannel(info.id)->getInfo());
				channelInfo->setName(info.name);
				channelInfo->setPublicLogin(info.publicLogin);
				_foundClients.append(getChannel(info.id));
			}

		}

		in >> groups;
		for (int i = 0; i < groups; i++)
		{
			advanGroupInfo info;
			in >> info;

			if (getGroup(info.id) == NULL)
			{
				GroupInfo *groupInfo = new GroupInfo(info.id, info.name);
				groupInfo->setPublicLogin(info.publicLogin);
				Group *group = new Group(groupInfo);
				_listOfClient.append(group);
				_foundClients.append(group);
			}
			else
			{
				GroupInfo *groupInfo = dynamic_cast<GroupInfo*>(getGroup(info.id)->getInfo());
				groupInfo->setName(info.name);
				groupInfo->setPublicLogin(info.publicLogin);
				_foundClients.append(getGroup(info.id));
			}
		}
	}

	case ServerCommand::servSendNewUserInfo:
	{
		unsigned int id;
		in >> id;
		advanUserInfo userInfoStruct;
		in >> userInfoStruct;
		if (getUser(id) == NULL)
		{
			UserInfo *userInfo = new UserInfo(id);
			userInfo->setName(userInfoStruct.name);
			userInfo->setLogin(userInfoStruct.login);
			userInfo->setDesc(userInfoStruct.desc);
			userInfo->setNubmer(userInfoStruct.number);
			User *user = new User(userInfo);
			_listOfClient.append(user);
		}
		else
		{
			UserInfo *userInfo = dynamic_cast<UserInfo*>(getUser(id)->getInfo());
			userInfo->setName(userInfoStruct.name);
			userInfo->setLogin(userInfoStruct.login);
			userInfo->setDesc(userInfoStruct.desc);
			userInfo->setNubmer(userInfoStruct.number);
		}

		emit loadedNewClientInfo(id, 1);
		break;
	}

	case ServerCommand::servSendNewChannelInfo:
	{
		advanChannelInfo channelInfoStruct;
		in >> channelInfoStruct;
		if (_listOfAdminChannel.contains(channelInfoStruct.id))
		{
			return;
		}
		ChannelInfo *channelInfo = dynamic_cast<ChannelInfo*>(getChannel(channelInfoStruct.id)->getInfo());
		channelInfo->setName(channelInfoStruct.name);
		channelInfo->setDesc(channelInfoStruct.desc);
		channelInfo->setCountOfSubs(channelInfoStruct.countSubs);
		channelInfo->setPublic(channelInfoStruct.isPublic);
		if (channelInfoStruct.isPublic)
			channelInfo->setPublicLogin(channelInfoStruct.publicLogin);

		emit loadedNewClientInfo(channelInfoStruct.id, 2);
		break;
	}

	case ServerCommand::servSendNewGroupInfo:
	{
		advanGroupInfo groupInfoStruct;
		in >> groupInfoStruct;
		if (_listOfAdminGroup.contains(groupInfoStruct.id))
		{
			return;
		}
		GroupInfo *groupInfo = dynamic_cast<GroupInfo*>(getGroup(groupInfoStruct.id)->getInfo());
		groupInfo->setName(groupInfoStruct.name);
		groupInfo->setDesc(groupInfoStruct.desc);
		groupInfo->setPublic(groupInfoStruct.isPublic);
		groupInfo->setCountOfUsers(groupInfoStruct.countUser);
		groupInfo->setAdminID(groupInfoStruct.adminID);
		groupInfo->setAdminName(groupInfoStruct.adminName);
		groupInfo->setPublic(groupInfoStruct.isPublic);
		if (groupInfoStruct.isPublic)
			groupInfo->setPublicLogin(groupInfoStruct.publicLogin);

		emit loadedNewClientInfo(groupInfoStruct.id, 3);
		break;
	}

	case ServerCommand::servDeleteSubFromChannel:
	{
		unsigned int userID;
		in >> userID;
		unsigned int channelID;
		in >> channelID;

		if (userID == _userInfo->getID())
		{
			_listOfChannel.removeOne(channelID);
			emit deletedFromClient(channelID, 2);
		}
		else
		{
			dynamic_cast<ChannelInfo*>(Connector::getInstance()->getChannel(channelID)->getInfo())->deleteUser(userID);
		}

		break;
	}

	case ServerCommand::servDeleteUserFromGroup:
	{
		unsigned int userID;
		in >> userID;
		unsigned int groupID;
		in >> groupID;

		if (userID == _userInfo->getID())
		{
			_listOfGroup.removeOne(groupID);
			emit deletedFromClient(groupID, 3);
		}
		else
		{
			dynamic_cast<GroupInfo*>(Connector::getInstance()->getGroup(groupID)->getInfo())->deleteUser(userID);
		}

		break;
	}

	case ServerCommand::servInviteUserToGroup:
	{
		advanUserInfo info;
		in >> info;
		unsigned int groupID;
		in >> groupID;
		QString groupName;
		in >> groupName;

		qDebug() << "servInviteUserToGroup";
		if (info.id == _userInfo->getID())
		{
			GroupInfo *groupInfo;
			if (getGroup(groupID) == NULL)
			{
				groupInfo = new GroupInfo(groupID, groupName);
				Group *group = new Group(groupInfo);
				_listOfClient.append(group);
			}
			else
			{
				groupInfo = dynamic_cast<GroupInfo*>(getGroup(groupID)->getInfo());
			}
			_listOfGroup.append(groupID);
			emit addedByClient(groupID, groupInfo);
		}
		else
		{
			UserInfo *userInfo;
			if (getUser(info.id) == NULL)
			{
				userInfo = new UserInfo(info.id);
				userInfo->setName(info.name);
				userInfo->setLogin(info.login);
				userInfo->setDesc(info.desc);
				userInfo->setNubmer(info.number);
				User *user = new User(userInfo);
				_listOfClient.append(user);
			}
			else
			{
				userInfo = dynamic_cast<UserInfo*>(getUser(info.id)->getInfo());
				userInfo->setName(info.name);
				userInfo->setLogin(info.login);
				userInfo->setDesc(info.desc);
				userInfo->setNubmer(info.number);
			}

			dynamic_cast<GroupInfo*>(Connector::getInstance()->getGroup(groupID)->getInfo())->addNewUser(userInfo);
		}

		break;
	}

	case ServerCommand::servInviteUserToChannel:
	{
		advanUserInfo info;
		in >> info;
		unsigned int channelID;
		in >> channelID;
		QString channelName;
		in >> channelName;

		if (info.id == _userInfo->getID())
		{
			ChannelInfo *channelInfo;
			if (getGroup(channelID) == NULL)
			{
				channelInfo = new ChannelInfo(channelID, channelName);
				Channel *channel = new Channel(channelInfo);
				_listOfClient.append(channel);
			}
			else
			{
				channelInfo = dynamic_cast<ChannelInfo*>(getGroup(channelID)->getInfo());
			}
			_listOfChannel.append(channelID);
			emit addedByClient(channelID, channelInfo);
		}
		else
		{
			UserInfo *userInfo;
			if (getUser(info.id) == NULL)
			{
				userInfo = new UserInfo(info.id);
				userInfo->setName(info.name);
				userInfo->setLogin(info.login);
				userInfo->setDesc(info.desc);
				userInfo->setNubmer(info.number);
				User *user = new User(userInfo);
				_listOfClient.append(user);
			}
			else
			{
				userInfo = dynamic_cast<UserInfo*>(getUser(info.id)->getInfo());
				userInfo->setName(info.name);
				userInfo->setLogin(info.login);
				userInfo->setDesc(info.desc);
				userInfo->setNubmer(info.number);
			}

			dynamic_cast<ChannelInfo*>(Connector::getInstance()->getChannel(channelID)->getInfo())->addNewUser(userInfo);
		}

		break;
	}

	case ServerCommand::servDeleteChannel:
	{
		unsigned int id;
		in >> id;
		_listOfChannel.removeOne(id);
		deleteChannel(id);

		emit deletedClient(id, 2);
		break;
	}

	case ServerCommand::servDeleteGroup:
	{
		unsigned int id;
		in >> id;
		_listOfGroup.removeOne(id);
		deleteGroup(id);

		emit deletedClient(id, 3);
		break;
	}

	case ServerCommand::servLeaveGroup:
	{
		unsigned int userID;
		in >> userID;
		unsigned int groupID;
		in >> groupID;

		dynamic_cast<GroupInfo*>(getGroup(groupID)->getInfo())->deleteUser(userID);
		break;
	}

	case ServerCommand::servLeaveChannel:
	{
		unsigned int userID;
		in >> userID;
		unsigned int channelID;
		in >> channelID;

		dynamic_cast<ChannelInfo*>(getChannel(channelID)->getInfo())->deleteUser(userID);
		break;
	}

	case ServerCommand::servDeleteMessage:
	{
		unsigned int senderID;
		in >> senderID;
		QDateTime date;
		in >> date;
		int type;
		in >> type;

		if (senderID != _userInfo->getID())
		{
			switch (type)
			{
			case 1:
			{
				Connector::getInstance()->getUser(senderID)->getChat()->deleteMessage(senderID, date);
				break;
			}
			case 2:
			{
				Connector::getInstance()->getChannel(senderID)->getChat()->deleteMessage(senderID, date);
				break;
			}
			case 3:
			{
				unsigned int id;
				in >> id;
				Connector::getInstance()->getGroup(id)->getChat()->deleteMessage(senderID, date);
				break;
			}
			}
		}

		emit deletedMessage(senderID, date);
		break;
	}

	case ServerCommand::servEditMessage:
	{
		unsigned int senderID;
		in >> senderID;
		QString newtext;
		in >> newtext;
		QDateTime date;
		in >> date;
		int type;
		in >> type;

		Message *msg = nullptr;
		if (senderID != _userInfo->getID())
		{
			switch (type)
			{
			case 1:
			{
				msg = Connector::getInstance()->getUser(senderID)->getChat()->getMessage(senderID, date);
				msg->setText(newtext);
				break;
			}
			case 2:
			{
				msg = Connector::getInstance()->getChannel(senderID)->getChat()->getMessage(senderID, date);
				msg->setText(newtext);
				break;
			}
			case 3:
			{
				unsigned int id;
				in >> id;
				msg = Connector::getInstance()->getGroup(id)->getChat()->getMessage(senderID, date);
				msg->setText(newtext);
				break;
			}
			}
		}
		if (msg != nullptr)
			emit changedMessage(msg);

		break;
	}

	case ServerCommand::servStartNewDialog:
	{
		advanUserInfo info;
		in >> info;

		UserInfo *userInfo;
		if (getUser(info.id) == NULL)
		{
			userInfo = new UserInfo(info.id);
			userInfo->setName(info.name);
			userInfo->setLogin(info.login);
			userInfo->setDesc(info.desc);
			userInfo->setNubmer(info.number);
			User *user = new User(userInfo);
			_listOfClient.append(user);
		}
		else
		{
			userInfo = dynamic_cast<UserInfo*>(getUser(info.id)->getInfo());
			userInfo->setName(info.name);
			userInfo->setLogin(info.login);
			userInfo->setDesc(info.desc);
			userInfo->setNubmer(info.number);
		}
		_listOfDialogsUsers.append(info.id);

		emit addedByClient(info.id, userInfo);

		break;
	}

	case ServerCommand::servAddToFriendsList:
	{
		advanUserInfo info;
		in >> info;

		UserInfo *userInfo;
		if (getUser(info.id) == NULL)
		{
			userInfo = new UserInfo(info.id);
			userInfo->setName(info.name);
			userInfo->setLogin(info.login);
			userInfo->setDesc(info.desc);
			userInfo->setNubmer(info.number);
			User *user = new User(userInfo);
			_listOfClient.append(user);
		}
		else
		{
			userInfo = dynamic_cast<UserInfo*>(getUser(info.id)->getInfo());
			userInfo->setName(info.name);
			userInfo->setLogin(info.login);
			userInfo->setDesc(info.desc);
			userInfo->setNubmer(info.number);
		}
		_listOfFriends.append(getUser(info.id));
	}

	case ServerCommand::servDeleteFriend:
	{
		unsigned int id;
		in >> id;

		_listOfFriends.removeOne(getUser(id));
	}

	case ServerCommand::errorAuth:
	{
		emit errLogin();
		break;
	}
	case ServerCommand::errorRegLogin:
	{
		emit errRegistLoginExist();
		break;
	}
	case ServerCommand::errorUserIsNotExist:
	{
		break;
	}
	case ServerCommand::errorLoginIsExistAlready:
	{
		errorFromServer = true;
		break;
	}
	}
}

void Connector::registUser(QString login, QString password, QString name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientRegistration;
    out << login;
    out << password;
    out << name;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::loginUser(QString login, QString password)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientAuthorization;
    out << login;
    out << password;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::loadClientList()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientLoadClientList;
    out << _userInfo->getID();
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
	_socket->waitForReadyRead();
}

void Connector::loadDialogMessages(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientLoadDialogMessages;
    out << _userInfo->getID();
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
    qDebug() << id;
}

void Connector::loadGroupMessages(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientLoadGroupMessages;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::loadChannelMessages(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientLoadChannelMessages;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::sendDialogMessage(unsigned int reciverID, QString text, QDateTime time)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientSendMessageToUser;
    out << _userInfo->getName();
    out << _userInfo->getID();
    out << reciverID;
    out << text;
    out << time;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::sendGroupMessage(unsigned int groupID, QString text, QDateTime time)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientSendGroupMessage;
    out << _userInfo->getName();
    out << _userInfo->getID();
    out << groupID;
    out << text;
    out << time;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::sendChannelMessage(unsigned int channelID, QString text, QDateTime time)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientSendChannelMessage;
    out << getChannel(channelID)->getInfo()->getName();
    out << channelID;
    out << text;
    out << time;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::loadUserInfo(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientLoadUserInfo;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::loadChannelInfo(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientLoadChannelInfo;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::loadGroupInfo(unsigned int id)
{
	qDebug() << "loadGroupInfo";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientLoadGroupInfo;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);

	qDebug() << "CONNECOTR LOAD" << QThread::currentThreadId();

    _socket->waitForReadyRead(1000);
}

void Connector::loadListOfFriends()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientGetFriendsList;
    out << _userInfo->getID();
    out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::sendNewUserInfo(advanUserInfo info)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientSendNewUserInfo;
    out << _userInfo->getID();
    out << info;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    //_socket->waitForReadyRead();
}

void Connector::loadChannelSubs(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientLoadChannelSubs;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::loadGroupUsers(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientLoadGroupUsers;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::deleteSubFromChannel(unsigned int userID, unsigned int channelID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientDeleteSubFromChannel;
    out << userID;
    out << channelID;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
}

void Connector::deleteUserFromGroup(unsigned int userID, unsigned int groupID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientDeleteUserFromGroup;
    out << userID;
    out << groupID;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
}

void Connector::sendNewChannelInfo(unsigned int channelID, advanChannelInfo info)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientSendNewChannelInfo;
    out << channelID;
    out << info;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
	_socket->waitForReadyRead();
}

void Connector::sendNewGroupInfo(unsigned int channelID, advanGroupInfo info)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientSendNewGroupInfo;
    out << channelID;
    out << info;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
	_socket->waitForReadyRead();
}

void Connector::inviteUserToGroup(QString login, unsigned int groupID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientInviteUserToGroup;
    out << login;
    out << groupID;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::inviteUserToChannel(QString login, unsigned int channelID)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint16)0;
	out << (quint8)ServerCommand::clientInviteUserToChannel;
	out << login;
	out << channelID;
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	_socket->write(block);
	_socket->waitForReadyRead();
}

void Connector::sendNewChannel(advanChannelInfo info, QList<unsigned int> list)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientSendNewChannel;
    out << info;
    out << _userInfo->getID();
	for (int i = 0; i < list.count(); i++)
	{
		out << list[i];
	}
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::sendNewGroup(advanGroupInfo info, QList<unsigned int> list)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientSendNewGroup;
    out << info;
    out << _userInfo->getID();
    for(int i = 0; i < list.count(); i++)
    {
        out << list[i];
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
    _socket->waitForReadyRead();
}

void Connector::deleteChannel(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientDeleteChannel;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
}

void Connector::deleteGroup(unsigned int id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)ServerCommand::clientDeleteGroup;
    out << id;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
}

void Connector::leaveChannel(unsigned int id)
{
	_listOfChannel.removeOne(id);

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint16)0;
	out << (quint8)ServerCommand::clientLeaveChannel;
	out << id;
	out << _userInfo->getID();
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	_socket->write(block);
}

void Connector::leaveGroup(unsigned int id)
{
	_listOfGroup.removeOne(id);

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint16)0;
	out << (quint8)ServerCommand::clientLeaveGroup;
	out << id;
	out << _userInfo->getID();
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	_socket->write(block);
}

void Connector::search(QString login, bool users, bool channels, bool groups)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint16)0;
	out << (quint8)ServerCommand::clientSearch;
	out << login;
	out << users;
	out << channels;
	out << groups;
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	_socket->write(block);
	_socket->waitForReadyRead();
}

void Connector::startNewDialog(unsigned int id)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint16)0;
	out << (quint8)ServerCommand::clientStartNewDialog;
	out << id;
	out << _userInfo->getID();
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	_socket->write(block);
	//_socket->waitForReadyRead();
}

void Connector::joinToChannel(unsigned int id)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint16)0;
	out << (quint8)ServerCommand::clientJoinToChannel;
	out << id;
	out << _userInfo->getID();
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	_socket->write(block);
	//_socket->waitForReadyRead();
}

void Connector::joinToGroup(unsigned int id)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint16)0;
	out << (quint8)ServerCommand::clientJoinToGroup;
	out << id;
	out << _userInfo->getID();
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	_socket->write(block);
	//_socket->waitForReadyRead();
}

void Connector::deleteMessage(Message *msg, int type, unsigned int id)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint16)0;
	out << (quint8)ServerCommand::clientDeleteMessage;
	out << msg->getSenderID();
	out << type;
	out << msg->getTime();
	out << msg->getText();
	out << id;
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	_socket->write(block);
}

void Connector::editMessage(Message *msg, QString newText, int type, unsigned int id)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint16)0;
	out << (quint8)ServerCommand::clientEditMessage;
	out << msg->getSenderID();
	out << msg->getTime();
	out << msg->getText();
	out << newText;
	out << type;
	out << id;
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	_socket->write(block);
}

void Connector::addToFriendsList(unsigned int id)
{
	_listOfFriends.append(getUser(id));

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint16)0;
	out << (quint8)ServerCommand::clientAddToFriendsList;
	out << id;
	out << _userInfo->getID();
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	_socket->write(block);
}

void Connector::removeFromFriends(unsigned int id)
{
	_listOfFriends.removeOne(getUser(id));

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint16)0;
	out << (quint8)ServerCommand::clientDeleteFriend;
	out << id;
	out << _userInfo->getID();
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	_socket->write(block);
}

User* Connector::getUser(unsigned int id)
{
    for(unsigned int i = 0;i != _listOfClient.count(); i++)
    {
        if((_listOfClient[i]->getInfo()->getID() == id) && (typeid (*_listOfClient[i]) == typeid (User)))
        {
            return dynamic_cast<User*>(_listOfClient[i]);
        }
    }
    return NULL;
}

User* Connector::getUser(QString login)
{
    for(unsigned int i = 0;i != _listOfClient.count(); i++)
    {
        if(typeid (*_listOfClient[i]) == typeid (User))
        {
            QString temp = dynamic_cast<UserInfo*>(_listOfClient[i]->getInfo())->getLogin();
            if(temp == login)
            {
                return dynamic_cast<User*>(_listOfClient[i]);
            }
        }
    }
    return NULL;
}

Group* Connector::getGroup(unsigned int id)
{
    for(unsigned int i = 0;i != _listOfClient.count(); i++)
    {
        if((_listOfClient[i]->getInfo()->getID() == id) && (typeid (*_listOfClient[i]) == typeid (Group)))
        {
            return dynamic_cast<Group*>(_listOfClient[i]);
        }
    }
    return NULL;
}

Channel* Connector::getChannel(unsigned int id)
{
    for(unsigned int i = 0;i != _listOfClient.count(); i++)
    {
        if((_listOfClient[i]->getInfo()->getID() == id) && (typeid (*_listOfClient[i]) == typeid (Channel)))
        {
            return dynamic_cast<Channel*>(_listOfClient[i]);
        }
    }
    return NULL;
}

void Connector::deleteClientFromList(Client* client)
{
	_listOfClient.removeOne(client);
	delete client;
}

void Connector::deleteClientFromList(unsigned int id, int type)
{

	switch (type)
	{
	case 1:
	{
		for (unsigned int i = 0; i != _listOfClient.count(); i++)
		{
			if ((_listOfClient[i]->getInfo()->getID() == id) && (typeid (*_listOfClient[i]) == typeid (User)))
			{
				Client* client = _listOfClient[i];
				_listOfClient.removeAt(i);
				delete client;
				return;
			}
		}
		break;
	}
	case 2:
	{
		for (unsigned int i = 0; i != _listOfClient.count(); i++)
		{
			if ((_listOfClient[i]->getInfo()->getID() == id) && (typeid (*_listOfClient[i]) == typeid (Channel)))
			{
				Client* client = _listOfClient[i];
				_listOfClient.removeAt(i);
				delete client;
				return;
			}
		}
		break;
	}
	case 3:
	{
		for (unsigned int i = 0; i != _listOfClient.count(); i++)
		{
			if ((_listOfClient[i]->getInfo()->getID() == id) && (typeid (*_listOfClient[i]) == typeid (Group)))
			{
				Client* client = _listOfClient[i];
				_listOfClient.removeAt(i);
				delete client;
				return;
			}
		}
		break;
	}
	}
}

bool Connector::isSubOfChannel(unsigned int id)
{
	return _listOfChannel.contains(id);
}

void Connector::addNewChannelJoin(unsigned int id)
{
	_listOfChannel.append(id);
}

bool Connector::isUserOfGroup(unsigned int id)
{
	return _listOfGroup.contains(id);
}

void Connector::addNewGroupJoin(unsigned int id)
{
	_listOfGroup.append(id);
}

bool Connector::hasDialogWithUser(unsigned int id)
{
	return _listOfDialogsUsers.contains(id);
}

void Connector::addNewDialogUser(unsigned int id)
{
	_listOfDialogsUsers.append(id);
}

QList<Client*> Connector::getFoundClients()
{
	return _foundClients;
}

bool Connector::isAdminOfChannel(unsigned int channelID)
{
    return _listOfAdminChannel.contains(channelID);
}

bool Connector::isAdminOfGroup(unsigned int groupID)
{
    return _listOfAdminGroup.contains(groupID);

}

bool Connector::isFriend(unsigned int id)
{
	return _listOfFriends.contains(getUser(id));
}

QList<Client*> Connector::getListOfClient()
{
    return _listOfClient;
}

UserInfo* Connector::getCurrentUserInfo()
{
    return _userInfo;
}

QList<User*> Connector::getListOfFriends()
{
    return _listOfFriends;
}

bool Connector::isLogined()
{
	return logined;
}

void Connector::errorChecked()
{
	errorFromServer = false;
}

bool Connector::isErrorFromServer()
{
	return errorFromServer;
}

unsigned int Connector::getNewGroupID()
{
	return newGroupID;
}

//ClientInfo* Connector::getClientInfo(unsigned int id)
//{
//    return mapOfClient.value(id)->getInfo();
//}

//unsigned int Connector::getClientID(int i)
//{
//    QMap<unsigned int, Client*>::iterator iter;
//    iter = instance->mapOfClient.begin();
//    iter = iter + i;

//    return iter.key();
//}

//Chat* Connector::getClientChat(unsigned int id)
//{
//    return mapOfClient.value(id)->getChat();
//}

//int Connector::getCountOfClient()
//{
//    return mapOfClient.count();
//}

void Connector::deinit()
{
    for(unsigned int i = 0;i < instance->_listOfClient.count(); i++)
    {
        delete instance->_listOfClient[i];

    }
    if(instance->_userInfo)
    {
        delete instance->_userInfo;
    }

    instance->_socket->disconnectFromHost();

    delete instance;
}
