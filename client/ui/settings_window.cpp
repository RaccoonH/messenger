#include "settings_window.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QDialog (parent)
{
    resize(500,350);

    QVBoxLayout *thisLayout = new QVBoxLayout(this);
    _stackedWidget = new QStackedWidget(this);
    thisLayout->addWidget(_stackedWidget);
    setLayout(thisLayout);

    _settings = new QWidget(_stackedWidget);
    _stackedWidget->addWidget(_settings);
    QVBoxLayout *settingsLayout = new QVBoxLayout(_settings);

    QPushButton *friendsButton = new QPushButton("Friends", _settings);
    QPushButton *searchButton = new QPushButton("Search", _settings);
    QPushButton *newChannelButton = new QPushButton("Create new channel", _settings);
    QPushButton *newGroupButton = new QPushButton("Create new group", _settings);
    QPushButton *aboutMeButton = new QPushButton("About me", _settings);

    settingsLayout->addWidget(friendsButton);
    settingsLayout->addWidget(searchButton);
    settingsLayout->addWidget(newChannelButton);
    settingsLayout->addWidget(newGroupButton);
    settingsLayout->addWidget(aboutMeButton);

    connect(friendsButton, &QPushButton::clicked, this, &SettingsWindow::onFriendsButtonClicked);
	connect(searchButton, &QPushButton::clicked, this, &SettingsWindow::onSearchSettingsClicked);
    connect(newChannelButton, &QPushButton::clicked, this, &SettingsWindow::onNewChannelClicked);
    connect(newGroupButton, &QPushButton::clicked, this, &SettingsWindow::onNewGroupClicked);
    connect(aboutMeButton, &QPushButton::clicked, this, &SettingsWindow::onAboutMeClicked);

}

void SettingsWindow::onFriendsButtonClicked()
{
    Connector::getInstance()->loadListOfFriends();

    if(_friends != nullptr)
    {
        delete _friends;
    }
    _friends = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(_friends);
    QPushButton *backToSettings = new QPushButton("Back");
    QScrollArea *usersArea = new QScrollArea(this);
    QWidget *usersAreaWidget = new QWidget(usersArea);
    QVBoxLayout *usersLayout = new QVBoxLayout(usersAreaWidget);

    usersLayout->setSpacing(2);
    usersLayout->setMargin(0);
    usersLayout->setAlignment(Qt::AlignBottom);
    usersLayout->setDirection(QBoxLayout::BottomToTop);
    usersLayout->setSizeConstraint(QLayout::SetFixedSize);

    for(int i = 0;i < Connector::getInstance()->getListOfFriends().count(); i++)
    {
		Client *client = Connector::getInstance()->getListOfFriends()[i];
		ClientLabel *clientLabel = new ClientLabel(client->getInfo()->getID(), client->getInfo(), usersAreaWidget);
		clientLabel->setBackGroundColor();
		_listOfFriednLabels.append(clientLabel);
		connect(clientLabel, &ClientLabel::clicked, this, &SettingsWindow::onFriendListClicked);
		usersLayout->addWidget(clientLabel);
    }

    usersArea->setWidget(usersAreaWidget);
    layout->addWidget(backToSettings);
    layout->addWidget(usersArea);

	QPushButton *infoFriendButton = new QPushButton("Info", _friends);
	connect(infoFriendButton, &QPushButton::clicked , this, &SettingsWindow::onFriendsInfoClicked);
	layout->addWidget(infoFriendButton);

    _stackedWidget->addWidget(_friends);
    _stackedWidget->setCurrentWidget(_friends);

    connect(backToSettings, &QPushButton::clicked, this, &SettingsWindow::backToSettings);

    setWindowTitle("Friends");
}

void SettingsWindow::onFriendListClicked(unsigned int id, int type)
{
	_currentFriendsID = id;
	for (int i = 0; i < _listOfFriednLabels.count(); i++)
	{
		if (_listOfFriednLabels[i]->getID() == id)
		{
			_listOfFriednLabels[i]->fillLabel();
		}
		else
		{
			_listOfFriednLabels[i]->unfillLabel();
		}
	}
}

void SettingsWindow::onFriendsInfoClicked()
{
	ClientInfo *info;
	
	if (_currentFriendsID == 0)
		return;

	info = dynamic_cast<UserInfo*>(Connector::getInstance()->getUser(_currentFriendsID)->getInfo());
	ClientInfoWidnow window(info, _friends);
	window.exec();
}

void SettingsWindow::onAboutMeClicked()
{
    if(_aboutMe != nullptr)
    {
        delete _aboutMe;
    }
    _aboutMe = new QWidget(this);
    QGridLayout *layout = new QGridLayout(_aboutMe);
    layout->setAlignment(Qt::AlignTop);

    QLabel *headline = new QLabel("About me\n", _aboutMe);
    QFont headlineFont;
    headlineFont.setPixelSize(20);
    headline->setFont(headlineFont);

    QFont textFont;
    textFont.setPixelSize(15);
    QLabel *name = new QLabel("Name: ", _aboutMe);
    QLineEdit *nameEdit = new QLineEdit(Connector::getInstance()->getCurrentUserInfo()->getName(), _aboutMe);
    QLabel *desc = new QLabel("Description: ", _aboutMe);
    QLineEdit *descEdit = new QLineEdit(Connector::getInstance()->getCurrentUserInfo()->getDesc(), _aboutMe);
    QLabel *login = new QLabel("Login: ", _aboutMe);
    QLineEdit *loginEdit = new QLineEdit(Connector::getInstance()->getCurrentUserInfo()->getLogin(), _aboutMe);
	loginEdit->setReadOnly(true);
    QLabel *number = new QLabel("Number: ", _aboutMe);
    QLineEdit *numberEdit = new QLineEdit(Connector::getInstance()->getCurrentUserInfo()->getNumber(), _aboutMe);

	_errorLabelAboutMe = new QLabel(_aboutMe);
	_errorLabelAboutMe->hide();

    QPushButton *backToSettings = new QPushButton("Back", _aboutMe);
    connect(backToSettings, &QPushButton::clicked, this, &SettingsWindow::backToSettings);
    QPushButton *applyButton = new QPushButton("Apply", _aboutMe);
    connect(applyButton, &QPushButton::clicked, this, &SettingsWindow::onApplySettingsClicked);


    layout->addWidget(headline, 0, 0);
    layout->addWidget(name, 1, 0);
    layout->addWidget(nameEdit, 1, 1);
    layout->addWidget(desc, 2, 0);
    layout->addWidget(descEdit, 2, 1);
    layout->addWidget(login, 3, 0);
    layout->addWidget(loginEdit, 3, 1);
    layout->addWidget(number, 4, 0);
    layout->addWidget(numberEdit, 4, 1);
	layout->addWidget(_errorLabelAboutMe, 5, 0);
    layout->addWidget(backToSettings, 6, 0);
    layout->addWidget(applyButton, 6, 1);

    setWindowTitle("About me");
    _stackedWidget->addWidget(_aboutMe);
    _stackedWidget->setCurrentWidget(_aboutMe);
}

void SettingsWindow::onApplySettingsClicked()
{
    advanUserInfo info;
    info.name = dynamic_cast<QLineEdit*>(dynamic_cast<QGridLayout*>(_aboutMe->layout())->itemAtPosition(1,1)->widget())->text();
    info.desc = dynamic_cast<QLineEdit*>(dynamic_cast<QGridLayout*>(_aboutMe->layout())->itemAtPosition(2,1)->widget())->text();
    info.login = dynamic_cast<QLineEdit*>(dynamic_cast<QGridLayout*>(_aboutMe->layout())->itemAtPosition(3,1)->widget())->text();
    info.number = dynamic_cast<QLineEdit*>(dynamic_cast<QGridLayout*>(_aboutMe->layout())->itemAtPosition(4,1)->widget())->text();

    if(!(info.name.isEmpty() && info.desc.isEmpty() && info.login.isEmpty() && info.number.isEmpty()))
    {
		if (info.name.count() < 3)
		{
			_errorLabelAboutMe->setText("Name is too short");
			_errorLabelAboutMe->show();
			return;
		}
        Connector::getInstance()->sendNewUserInfo(info);
        Connector::getInstance()->getCurrentUserInfo()->setName(info.name);
        Connector::getInstance()->getCurrentUserInfo()->setDesc(info.desc);
        Connector::getInstance()->getCurrentUserInfo()->setNubmer(info.number);
    }

    backToSettings();
}

void SettingsWindow::onSearchSettingsClicked()
{
	if (_search != nullptr)
	{
		delete _search;
	}
	_search = new QWidget(this);
	QGridLayout *layout = new QGridLayout(_search);
	QPushButton *searchButton = new QPushButton("Search", _search);
	connect(searchButton, &QPushButton::clicked, this, &SettingsWindow::onSearchClicked);
	_searchLineEdit = new QLineEdit(_search);
	_usersSearch = new QCheckBox("Users", _search);
	_channelsSearch = new QCheckBox("Channels", _search);
	_groupsSearch = new QCheckBox("Groups", _search);

	_clientArea = new QScrollArea(_search);
	_clientAreaWidget = new QWidget(_clientArea);
	_clientLayout = new QVBoxLayout(_clientAreaWidget);
	_clientLayout->setAlignment(Qt::AlignTop);
	_clientLayout->setSpacing(2);
	_clientLayout->setMargin(0);
	_clientLayout->setSizeConstraint(QLayout::SetFixedSize);
	_clientArea->setWidget(_clientAreaWidget);
	_clientArea->setWidgetResizable(true);

	QPushButton *infoButton = new QPushButton("Info", this);

	layout->addWidget(_searchLineEdit, 0, 0, 1, 2);
	layout->addWidget(searchButton, 0, 2);
	layout->addWidget(_usersSearch, 1, 0);
	layout->addWidget(_channelsSearch, 1, 1);
	layout->addWidget(_groupsSearch, 1, 2);
	layout->addWidget(_clientArea, 2, 0, 1, 3);
	layout->addWidget(infoButton, 3, 0);
	connect(infoButton, &QPushButton::clicked, this, &SettingsWindow::onInfoClicked);

	setWindowTitle("Search");
	_stackedWidget->addWidget(_search);
	_stackedWidget->setCurrentWidget(_search);
}


void SettingsWindow::onSearchClicked()
{
	QLayoutItem* item;
	while ((item = _clientLayout->layout()->takeAt(0)) != NULL)
	{
		delete item->widget();
		delete item;
	}
	_listOfClientsLabel.clear();
	Connector::getInstance()->search(_searchLineEdit->text(),
		_usersSearch->isChecked(),
		_channelsSearch->isChecked(),
		_groupsSearch->isChecked());
	int count = Connector::getInstance()->getFoundClients().count();
	for (int i = 0; i < count; i++)
	{
		Client *client = Connector::getInstance()->getFoundClients()[i];
		ClientLabel *clientLabel = new ClientLabel(client->getInfo()->getID(), client->getInfo(), _clientAreaWidget);
		clientLabel->showLogin();
		clientLabel->setBackGroundColor();
		_listOfClientsLabel.append(clientLabel);
		connect(clientLabel, &ClientLabel::clicked, this, &SettingsWindow::onFoundClientLabelClicked);
		_clientLayout->addWidget(clientLabel);
	}
}

void SettingsWindow::onFoundClientLabelClicked(unsigned int id, int type)
{
	_currentFoundClientID = id;
	_currentFoundClientType = type;
	for (int i = 0; i < _listOfClientsLabel.count(); i++)
	{
		if (_listOfClientsLabel[i]->getID() == id && _listOfClientsLabel[i]->getType() == type)
		{
			_listOfClientsLabel[i]->fillLabel();
		}
		else
		{
			_listOfClientsLabel[i]->unfillLabel();
		}
	}
}

void SettingsWindow::onInfoClicked()
{
	ClientInfo *info;
	switch (_currentFoundClientType)
	{
	case 0:
		return;

	case 1:
	{
		Connector::getInstance()->loadUserInfo(_currentFoundClientID);
		info = Connector::getInstance()->getUser(_currentFoundClientID)->getInfo();
		break;
	}

	case 2:
	{
		Connector::getInstance()->loadChannelInfo(_currentFoundClientID);
		info = Connector::getInstance()->getChannel(_currentFoundClientID)->getInfo();
		break;
	}

	case 3:
	{
		Connector::getInstance()->loadGroupInfo(_currentFoundClientID);
		info = Connector::getInstance()->getGroup(_currentFoundClientID)->getInfo();
		break;
	}

	}

	ClientInfoWidnow window(info, _search);
	connect(&window, &ClientInfoWidnow::addedNewClient, this, &SettingsWindow::onAddedNewClient);
	window.exec();
}

void SettingsWindow::onAddedNewClient(unsigned int id, int type)
{
	ClientInfo *info;
	switch (type)
	{
	case 1:
	{
		info = Connector::getInstance()->getUser(id)->getInfo();
		emit addedNewClient(id, info);
		Connector::getInstance()->addNewDialogUser(id);
		
		break;
	}
	case 2:
	{
		info = Connector::getInstance()->getChannel(id)->getInfo();
		emit addedNewClient(id, info);
		Connector::getInstance()->addNewChannelJoin(id);
		break;
	}
	case 3:
	{
		info = Connector::getInstance()->getGroup(id)->getInfo();
		emit addedNewClient(id, info);
		Connector::getInstance()->addNewGroupJoin(id);
		break;
	}
	}

	//emit addedNewClient(id, type);
}

void SettingsWindow::onNewChannelClicked()
{
    if(_newChannel != nullptr)
    {
        delete _newChannel;
    }
    _newChannel = new QWidget(this);
    QGridLayout *layout = new QGridLayout(_newChannel);
    layout->setAlignment(Qt::AlignTop);

    QLabel *headline = new QLabel("Create new channel\n", _newChannel);
    QFont headlineFont;
    headlineFont.setPixelSize(20);
    headline->setFont(headlineFont);

    QFont textFont;
    textFont.setPixelSize(15);
    QLabel *name = new QLabel("Name: ", _newChannel);
    _nameEditNewChannel = new QLineEdit(_newChannel);
    QLabel *desc = new QLabel("Description: ", _newChannel);
    _descEditNewChannel = new QLineEdit(_newChannel);
    _isPublicButtonNewChannel = new QRadioButton("Public", _newChannel);
	connect(_isPublicButtonNewChannel, &QRadioButton::toggled, this, &SettingsWindow::onPublicChannelToggled);

	_publicLoginNewChannelLabel = new QLabel("Public login: ", _newChannel);
	_publicLoginNewChannelLabel->hide();
	_publicLoginNewChannel = new QLineEdit(_newChannel);
	_publicLoginNewChannel->hide();
	

	QScrollArea *usersArea = new QScrollArea(_newGroup);
	QWidget *usersAreaWidget = new QWidget(usersArea);
	QVBoxLayout *usersLayout = new QVBoxLayout(usersAreaWidget);

	QLabel *inviteFriendsLabel = new QLabel("Invite your friends: ", _newGroup);
	for (int i = 0; i < Connector::getInstance()->getListOfFriends().count(); i++)
	{
		UserInfo *info = dynamic_cast<UserInfo*>(Connector::getInstance()->getListOfFriends()[i]->getInfo());
		UserLabel *user = new UserLabel(info->getID(), info->getName(), usersAreaWidget);
		connect(user, &UserLabel::clicked, this, &SettingsWindow::onFriendLabelClickedChannel);
		_mapOfUsersLabelChannel.insert(info->getID(), user);
		usersLayout->addWidget(user);
	}
	usersArea->setWidget(usersAreaWidget);

	_errorLabelChannel = new QLabel(_newChannel);
	_errorLabelChannel->hide();

    QPushButton *backToSettings = new QPushButton("Back", _newChannel);
    connect(backToSettings, &QPushButton::clicked, this, &SettingsWindow::backToSettings);
    QPushButton *applyButton = new QPushButton("Apply", _newChannel);
    connect(applyButton, &QPushButton::clicked, this, &SettingsWindow::onApplyNewChannelClicked);

    layout->addWidget(headline, 0, 0);
    layout->addWidget(name, 1, 0);
    layout->addWidget(_nameEditNewChannel, 1, 1);
    layout->addWidget(desc, 2, 0);
    layout->addWidget(_descEditNewChannel, 2, 1);
    layout->addWidget(_isPublicButtonNewChannel, 3, 0);
	layout->addWidget(_publicLoginNewChannelLabel, 4, 0);
	layout->addWidget(_publicLoginNewChannel, 4, 1);
	layout->addWidget(inviteFriendsLabel, 5, 0);
	layout->addWidget(usersArea, 6, 0, 1, 2);
	layout->addWidget(_errorLabelChannel, 7, 0);
	layout->addWidget(backToSettings, 8, 0);
	layout->addWidget(applyButton, 8, 1);

    setWindowTitle("Create new channel");
    _stackedWidget->addWidget(_newChannel);
    _stackedWidget->setCurrentWidget(_newChannel);
}

void SettingsWindow::onApplyNewChannelClicked()
{
    advanChannelInfo info;
    info.name = _nameEditNewChannel->text();
    info.desc = _descEditNewChannel->text();
    info.isPublic = _isPublicButtonNewChannel->isChecked();
    info.countSubs = 0;
	
	if (info.name.count() < 3)
	{
		_errorLabelChannel->setText("Name is too short");
		_errorLabelChannel->show();
		return;
	}

	if (info.isPublic)
	{
		if (_publicLoginNewChannel->text().count() < 3)
		{
			_errorLabelChannel->setText("Public login is too short");
			_errorLabelChannel->show();
			return;
		}
		else
		{
			info.publicLogin = _publicLoginNewChannel->text();
		}
	}

    Connector::getInstance()->sendNewChannel(info, _listOfInvitedFriendsChannel);
	if (Connector::getInstance()->isErrorFromServer())
	{
		_errorLabelChannel->setText("This login is already exists");
		_errorLabelChannel->show();
		Connector::getInstance()->errorChecked();
		return;
	}
    ChannelInfo* channelInfo = dynamic_cast<ChannelInfo*>(Connector::getInstance()->getListOfClient().last()->getInfo());
    channelInfo->setName(info.name);
    channelInfo->setDesc(info.desc);
    channelInfo->setPublic(info.isPublic);
	for (int i = 0; i < _listOfInvitedFriendsChannel.count(); i++)
	{
		ClientInfo *user = Connector::getInstance()->getUser(_listOfInvitedFriendsChannel[i])->getInfo();
		channelInfo->addNewUser(dynamic_cast<UserInfo*>(user));
	}
	channelInfo->addNewUser(Connector::getInstance()->getCurrentUserInfo());
	channelInfo->setCountOfSubs(_listOfInvitedFriendsChannel.count() + 1);

    backToSettings();
    emit createdNewChannel(channelInfo->getID(), channelInfo);
}

void SettingsWindow::onPublicChannelToggled()
{
	if (_publicLoginNewChannel->isHidden())
	{
		_publicLoginNewChannel->show();
		_publicLoginNewChannelLabel->show();
	}
	else
	{
		_publicLoginNewChannel->hide();
		_publicLoginNewChannelLabel->hide();
	}
}

void SettingsWindow::onNewGroupClicked()
{
    if(_newGroup != nullptr)
    {
        delete _newGroup;
    }
    if(Connector::getInstance()->getListOfFriends().count() == 0)
    {
        Connector::getInstance()->loadListOfFriends();
    }
    _newGroup = new QWidget(this);
    QGridLayout *layout = new QGridLayout(_newGroup);
    layout->setAlignment(Qt::AlignTop);

    QLabel *headline = new QLabel("Create new group\n", _newGroup);
    QFont headlineFont;
    headlineFont.setPixelSize(20);
    headline->setFont(headlineFont);

    QFont textFont;
    textFont.setPixelSize(15);
    QLabel *name = new QLabel("Name: ", _newGroup);
    _nameEditNewGroup = new QLineEdit(_newGroup);
    QLabel *desc = new QLabel("Description: ", _newGroup);
    _descEditNewGroup = new QLineEdit(_newGroup);
    _isPublicButtonNewGroup = new QRadioButton("Public", _newGroup);
	connect(_isPublicButtonNewGroup, &QRadioButton::toggled, this, &SettingsWindow::onPublicGroupToggled);

	_publicLoginNewGroupLabel = new QLabel("Public login: ", _newGroup);
	_publicLoginNewGroupLabel->hide();
	_publicLoginNewGroup = new QLineEdit(_newGroup);
	_publicLoginNewGroup->hide();

    QScrollArea *usersArea = new QScrollArea(_newGroup);
    QWidget *usersAreaWidget = new QWidget(usersArea);
    QVBoxLayout *usersLayout = new QVBoxLayout(usersAreaWidget);

    QLabel *inviteFriendsLabel = new QLabel("Invite your friends: ", _newGroup);
    for(int i = 0; i < Connector::getInstance()->getListOfFriends().count(); i++)
    {
        UserInfo *info = dynamic_cast<UserInfo*>(Connector::getInstance()->getListOfFriends()[i]->getInfo());
        UserLabel *user = new UserLabel(info->getID(), info->getName(), usersAreaWidget);
        connect(user, &UserLabel::clicked, this, &SettingsWindow::onFriendLabelClicked);
        _mapOfUsersLabel.insert(info->getID(), user);
        usersLayout->addWidget(user);
    }
    usersArea->setWidget(usersAreaWidget);

	_errorLabel = new QLabel(_newGroup);
	_newGroup->hide();

    QPushButton *backToSettings = new QPushButton("Back", _newGroup);
    connect(backToSettings, &QPushButton::clicked, this, &SettingsWindow::backToSettings);
    QPushButton *applyButton = new QPushButton("Apply", _newGroup);
    connect(applyButton, &QPushButton::clicked, this, &SettingsWindow::onApplyNewGroupClicked);

    layout->addWidget(headline, 0, 0);
    layout->addWidget(name, 1, 0);
    layout->addWidget(_nameEditNewGroup, 1, 1);
    layout->addWidget(desc, 2, 0);
    layout->addWidget(_descEditNewGroup, 2, 1);
    layout->addWidget(_isPublicButtonNewGroup, 3, 0);
	layout->addWidget(_publicLoginNewGroupLabel, 4, 0);
	layout->addWidget(_publicLoginNewGroup, 4, 1);
    layout->addWidget(inviteFriendsLabel, 5, 0);
    layout->addWidget(usersArea, 6, 0, 1, 2);
	layout->addWidget(_errorLabel, 7, 0);
    layout->addWidget(backToSettings, 8, 0);
    layout->addWidget(applyButton, 8, 1);

    setWindowTitle("Create new group");
    _stackedWidget->addWidget(_newGroup);
    _stackedWidget->setCurrentWidget(_newGroup);
}

void SettingsWindow::onFriendLabelClicked(unsigned int id)
{
    if(_mapOfUsersLabel.value(id)->isFill())
    {
        _listOfInvitedFriends.removeOne(id);
        _mapOfUsersLabel.value(id)->unfillLabel();
    }
    else
    {
        _listOfInvitedFriends.append(id);
        _mapOfUsersLabel.value(id)->fillLabel();
    }
}

void SettingsWindow::onFriendLabelClickedChannel(unsigned int id)
{
	if (_mapOfUsersLabelChannel.value(id)->isFill())
	{
		_listOfInvitedFriendsChannel.removeOne(id);
		_mapOfUsersLabelChannel.value(id)->unfillLabel();
	}
	else
	{
		_listOfInvitedFriendsChannel.append(id);
		_mapOfUsersLabelChannel.value(id)->fillLabel();
	}
}

void SettingsWindow::onPublicGroupToggled()
{
	if (_publicLoginNewGroup->isHidden())
	{
		_publicLoginNewGroupLabel->show();
		_publicLoginNewGroup->show();
	}
	else
	{
		_publicLoginNewGroupLabel->hide();
		_publicLoginNewGroup->hide();
	}
}

void SettingsWindow::onApplyNewGroupClicked()
{
    advanGroupInfo info;
    info.name = _nameEditNewGroup->text();
    info.desc = _descEditNewGroup->text();
    info.isPublic = _isPublicButtonNewGroup->isChecked();

	if (info.name.count() < 3)
	{
		_errorLabel->setText("Name is too short");
		_errorLabel->show();
		return;
	}

	if (info.isPublic)
	{
		if (_publicLoginNewGroup->text().count() < 3)
		{
			_errorLabel->setText("Public login is too short");
			_errorLabel->show();
			return;
		}
		else
		{
			info.publicLogin = _publicLoginNewGroup->text();
		}
	}

    Connector::getInstance()->sendNewGroup(info, _listOfInvitedFriends);

	if (Connector::getInstance()->isErrorFromServer())
	{
		_errorLabel->setText("This login is already exists");
		_errorLabel->show();
		Connector::getInstance()->errorChecked();
		return;
	}

	unsigned int id = Connector::getInstance()->getNewGroupID();

	GroupInfo* groupInfo = dynamic_cast<GroupInfo*>(Connector::getInstance()->getGroup(id)->getInfo());
    groupInfo->setName(info.name);
    groupInfo->setDesc(info.desc);
    for(int i = 0; i < _listOfInvitedFriends.count(); i++)
    {
        ClientInfo *user = Connector::getInstance()->getUser(_listOfInvitedFriends[i])->getInfo();
        groupInfo->addNewUser(dynamic_cast<UserInfo*>(user));
    }
	groupInfo->addNewUser(Connector::getInstance()->getCurrentUserInfo());
    groupInfo->setCountOfUsers(_listOfInvitedFriends.count() + 1);
    groupInfo->setPublic(info.isPublic);
	if (info.isPublic)
		groupInfo->setPublicLogin(info.publicLogin);
		

    backToSettings();
    emit createdNewChannel(groupInfo->getID(), groupInfo);
}

void SettingsWindow::backToSettings()
{
    _stackedWidget->setCurrentWidget(_settings);
}

SettingsWindow::~SettingsWindow()
{

}
