#include "ui/clientinfo_widnow.h"

ClientInfoWidnow::ClientInfoWidnow(ClientInfo *clientInfo, QWidget *parent) : QDialog (parent)
{
    resize(500,350);

	_info = clientInfo;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);

    QLabel *headline = new QLabel("Information\n",this);
    QFont headlineFont;
    headlineFont.setPixelSize(20);
    headline->setFont(headlineFont);

    QFont textFont;
    textFont.setPixelSize(15);
    QLabel *name = new QLabel(this) ;
    QLabel *desc = new QLabel(this);
    name->setText("Name: " + clientInfo->getName());
    desc->setText("Description: " + clientInfo->getDesc());

    layout->addWidget(headline);
    layout->addWidget(name);
    layout->addWidget(desc);

    if( typeid (*clientInfo) == typeid (UserInfo))
    {
        QLabel *login = new QLabel(this);
        login->setText("Login: " + dynamic_cast<UserInfo*>(clientInfo)->getLogin());
        layout->addWidget(login);

        QLabel *number = new QLabel(this);
        number->setText("Number: " + dynamic_cast<UserInfo*>(clientInfo)->getNumber());
        layout->addWidget(number);

		if (clientInfo->getID() != Connector::getInstance()->getCurrentUserInfo()->getID())
		{
			if (!Connector::getInstance()->hasDialogWithUser(clientInfo->getID()))
			{
				QPushButton *subButton = new QPushButton("Start the dialogue", this);
				connect(subButton, &QPushButton::clicked, this, &ClientInfoWidnow::onSubButtonClicked);
				layout->addWidget(subButton);
			}
			if (Connector::getInstance()->isFriend(clientInfo->getID()))
			{
				QPushButton *removeFromFriends = new QPushButton("Remove from friends", this);
				connect(removeFromFriends, &QPushButton::clicked, this, &ClientInfoWidnow::onRemoveFriendClicked);
				layout->addWidget(removeFromFriends);
			}
			else
			{
				QPushButton *addFriendButton = new QPushButton("Add to friends list", this);
				connect(addFriendButton, &QPushButton::clicked, this, &ClientInfoWidnow::onAddedFriendsClicked);
				layout->addWidget(addFriendButton);
			}
		}

        setWindowTitle("User information");
    }

    if(typeid (*clientInfo) == typeid (ChannelInfo))
    {
        QLabel *countOfSubs = new QLabel(this);
        countOfSubs->setText("Subscribers: " +
                             QString::number(dynamic_cast<ChannelInfo*>(clientInfo)->getCountOfSubs()));
        QLabel *isPublic = new QLabel(this);
        if(dynamic_cast<ChannelInfo*>(clientInfo)->isPublic())
        {
			QLabel *publicLogin = new QLabel(this);
			publicLogin->setText("Public login: " + dynamic_cast<ChannelInfo*>(clientInfo)->getPublicLogin());
            isPublic->setText("Access: Public");
			layout->addWidget(publicLogin);
        }
        else
        {
            isPublic->setText("Access: Private");
        }

		layout->addWidget(countOfSubs);
		layout->addWidget(isPublic);

		if (!Connector::getInstance()->isSubOfChannel(clientInfo->getID()))
		{
			QPushButton *subButton = new QPushButton("Subscribe to the channel", this);
			connect(subButton, &QPushButton::clicked, this, &ClientInfoWidnow::onSubButtonClicked);
			layout->addWidget(subButton);
		}
		else
		{
			if (!Connector::getInstance()->isAdminOfChannel(clientInfo->getID()))
			{
				QPushButton *leaveButton = new QPushButton("Leave this channel", this);
				connect(leaveButton, &QPushButton::clicked, this, &ClientInfoWidnow::onLeaveButtonClicked);
				layout->addWidget(leaveButton);
			}
		}
        setWindowTitle("Channel information");
    }

    if(typeid (*clientInfo) == typeid (GroupInfo))
    {
        QLabel *countOfUsers = new QLabel(this);
        countOfUsers->setText("Number of users: " +
                             QString::number(dynamic_cast<GroupInfo*>(clientInfo)->getCountOfUsers()));
        QLabel *isPublic = new QLabel(this);

        if(dynamic_cast<GroupInfo*>(clientInfo)->isPublic())
        {
			QLabel *publicLogin = new QLabel(this);
			publicLogin->setText("Public login: " + dynamic_cast<GroupInfo*>(clientInfo)->getPublicLogin());
            isPublic->setText("Access: Public");
			layout->addWidget(publicLogin);
        }
        else
        {
            isPublic->setText("Access: Private");
        }

        QLabel *admin = new QLabel(this);
        admin->setText("Admin: " + dynamic_cast<GroupInfo*>(clientInfo)->getAdminName());

        layout->addWidget(countOfUsers);
        layout->addWidget(isPublic);
        layout->addWidget(admin);

        QScrollArea *usersArea = new QScrollArea(this);
        QWidget *usersAreaWidget = new QWidget(usersArea);
        QVBoxLayout *usersLayout = new QVBoxLayout(usersAreaWidget);

        QLabel *users = new QLabel("Users: ", this);
        layout->addWidget(users);
        for(int i = 0; i < dynamic_cast<GroupInfo*>(clientInfo)->getListOfUser().count(); i++)
        {
            UserInfo *info = dynamic_cast<GroupInfo*>(clientInfo)->getListOfUser()[i];
            UserLabel *userLabel = new UserLabel(info->getID(), info->getName(), this);
            connect(userLabel, &UserLabel::clicked, this, &ClientInfoWidnow::onUserLabelClicked);
            usersLayout->addWidget(userLabel);
            _map.insert(info->getID(), userLabel);
        }

        QPushButton *infoButton = new QPushButton("User info", this);
		connect(infoButton, &QPushButton::clicked, this, &ClientInfoWidnow::onUserInfoClicked);

		usersArea->setWidget(usersAreaWidget);
		layout->addWidget(usersArea);
		layout->addWidget(infoButton);

		if (!Connector::getInstance()->isUserOfGroup(clientInfo->getID()))
		{
			QPushButton *subButton = new QPushButton("Join to the group", this);
			connect(subButton, &QPushButton::clicked, this, &ClientInfoWidnow::onSubButtonClicked);
			layout->addWidget(subButton);
		}
		else
		{
			if (!Connector::getInstance()->isAdminOfGroup(clientInfo->getID()))
			{
				QPushButton *leaveButton = new QPushButton("Leave this group", this);
				connect(leaveButton, &QPushButton::clicked, this, &ClientInfoWidnow::onLeaveButtonClicked);
				layout->addWidget(leaveButton);
			}
		}
        setWindowTitle("Group information");
    }
}

void ClientInfoWidnow::onUserLabelClicked(unsigned int id)
{
    QMap<unsigned int, UserLabel*>::iterator i;
    for(i = _map.begin(); i != _map.end(); ++i)
    {
        if(i.key() == id)
        {
            i.value()->fillLabel();
        }
        else
        {
            i.value()->unfillLabel();
        }
    }
    _currentID = id;
}

void ClientInfoWidnow::onUserInfoClicked()
{
	if (_currentID != 0)
	{
		Connector::getInstance()->loadUserInfo(_currentID);
		UserInfo *info = dynamic_cast<UserInfo*>(Connector::getInstance()->getUser(_currentID)->getInfo());
		ClientInfoWidnow *window = new ClientInfoWidnow(info, this);
		window->exec();
	}
}

void ClientInfoWidnow::onLeaveButtonClicked()
{
	if (typeid(ChannelInfo) == typeid(*_info))
	{
		//send Connector;
		unsigned int id = _info->getID();
		Connector::getInstance()->deleteClientFromList(_info->getID(), 2);
		_info = nullptr;
		Connector::getInstance()->leaveChannel(id);
		emit deletedClient(id, 2);
	}
	else
	{
		//send connector group;
		unsigned int id = _info->getID();
		Connector::getInstance()->deleteClientFromList(_info->getID(), 3);
		_info = nullptr;
		Connector::getInstance()->leaveGroup(id);
		emit deletedClient(id, 3);
	}
	this->reject();
}

void ClientInfoWidnow::onSubButtonClicked()
{
	if (typeid(*_info) == typeid(UserInfo))
	{
		Connector::getInstance()->startNewDialog(_info->getID());
		emit addedNewClient(_info->getID(), 1);
	}
	if (typeid(*_info) == typeid(ChannelInfo))
	{
		Connector::getInstance()->joinToChannel(_info->getID());
		emit addedNewClient(_info->getID(), 2);
	}
	if (typeid(*_info) == typeid(GroupInfo))
	{
		Connector::getInstance()->joinToGroup(_info->getID());
		emit addedNewClient(_info->getID(), 3);
	}
	this->reject();
}

void ClientInfoWidnow::onAddedFriendsClicked()
{
	Connector::getInstance()->addToFriendsList(_info->getID());
	this->reject();
}

void ClientInfoWidnow::onRemoveFriendClicked()
{
	Connector::getInstance()->removeFromFriends(_info->getID());
	this->reject();
}

ClientInfoWidnow::~ClientInfoWidnow()
{

}
