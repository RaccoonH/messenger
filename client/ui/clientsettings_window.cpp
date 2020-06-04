#include "clientsettings_window.h"

ClientSettingsWindow::ClientSettingsWindow(ClientInfo *clientInfo, QWidget *parent) : QDialog (parent)
{
    resize(500,350);
    QVBoxLayout *thisLayout = new QVBoxLayout(this);
    _stack = new QStackedWidget(this);
    thisLayout->addWidget(_stack);
    setLayout(thisLayout);

    _info = clientInfo;

    clientSettingsWidget = new QWidget(_stack);
    _stack->addWidget(clientSettingsWidget);

    if(clientInfo->getDesc() == nullptr)
    {
        if(typeid (*clientInfo) == typeid (ChannelInfo))
            Connector::getInstance()->loadChannelInfo(clientInfo->getID());
        else
            Connector::getInstance()->loadGroupInfo(clientInfo->getID());
    }

    QGridLayout *layout = new QGridLayout(clientSettingsWidget);
    layout->setAlignment(Qt::AlignTop);

    QLabel *headline = new QLabel("Settings\n",clientSettingsWidget);
    QFont headlineFont;
    headlineFont.setPixelSize(20);
    headline->setFont(headlineFont);

    QFont textFont;
    textFont.setPixelSize(15);
    QLabel *name = new QLabel("Name: ", clientSettingsWidget) ;
    _nameEdit = new QLineEdit(clientInfo->getName(), clientSettingsWidget);
    QLabel *desc = new QLabel("Description: ", clientSettingsWidget);
    _descEdit = new QLineEdit(clientInfo->getDesc(), clientSettingsWidget);
    _errorLabel = new QLabel(clientSettingsWidget);
    _errorLabel->hide();

    if(typeid (*clientInfo) == typeid (ChannelInfo))
    {
        QPushButton *showUsers = new QPushButton("Show users", clientSettingsWidget);
        QPushButton *deleteThisChannel = new QPushButton("Delete this channel", clientSettingsWidget);
        connect(deleteThisChannel, &QPushButton::clicked, this, &ClientSettingsWindow::onDeleteClientClicked);
        QPushButton *apply = new QPushButton("Apply", clientSettingsWidget);
        QPushButton *exit = new QPushButton("Exit", clientSettingsWidget);
        connect(exit, &QPushButton::clicked, this, &ClientSettingsWindow::onExitButtonClicked);
        connect(showUsers, &QPushButton::clicked, this, &ClientSettingsWindow::onShowChannelUsersButtonClicked);
        connect(apply, &QPushButton::clicked, this, &ClientSettingsWindow::onApplyButtonClicked);

		layout->addWidget(headline, 0, 0);
		layout->addWidget(name, 1, 0);
		layout->addWidget(_nameEdit, 1, 1);
		layout->addWidget(desc, 2, 0);
		layout->addWidget(_descEdit, 2, 1);

		_loginLabel = new QLabel("Public login: ", clientSettingsWidget);
		_loginEdit = new QLineEdit(clientSettingsWidget);
		
		_isPublicButton = new QRadioButton("Public", clientSettingsWidget);
		if (dynamic_cast<ChannelInfo*>(clientInfo)->isPublic())
		{
			_isPublicButton->setChecked(true);
			_loginEdit->setText(dynamic_cast<ChannelInfo*>(clientInfo)->getPublicLogin());
		}
		else
		{
			_isPublicButton->setChecked(false);
			_loginLabel->hide();
			_loginEdit->hide();
		}
		connect(_isPublicButton, &QRadioButton::toggled, this, &ClientSettingsWindow::onPublicButtonToggled);

		layout->addWidget(_isPublicButton, 3, 0);
		layout->addWidget(_loginLabel, 4, 0);
		layout->addWidget(_loginEdit, 4, 1);
		layout->addWidget(showUsers, 5, 0);
		layout->addWidget(deleteThisChannel, 6, 0);
		layout->addWidget(apply, 7, 0);
		layout->addWidget(exit, 7, 1);
		layout->addWidget(_errorLabel, 8, 0);

        setWindowTitle("Channel information");
    }

    if(typeid (*clientInfo) == typeid (GroupInfo))
    {
        QPushButton *showUsers = new QPushButton("Show users", clientSettingsWidget);
        QPushButton *deleteThisGroup = new QPushButton("Delete this group", clientSettingsWidget);
        connect(deleteThisGroup, &QPushButton::clicked, this, &ClientSettingsWindow::onDeleteClientClicked);
        QPushButton *apply = new QPushButton("Apply", clientSettingsWidget);
        QPushButton *exit = new QPushButton("Exit", clientSettingsWidget);
        connect(exit, &QPushButton::clicked, this, &ClientSettingsWindow::onExitButtonClicked);
        connect(showUsers, &QPushButton::clicked, this, &ClientSettingsWindow::onShowGroupUsersButtonClicked);
        connect(apply, &QPushButton::clicked, this, &ClientSettingsWindow::onApplyButtonClicked);

		layout->addWidget(headline, 0, 0);
		layout->addWidget(name, 1, 0);
		layout->addWidget(_nameEdit, 1, 1);
		layout->addWidget(desc, 2, 0);
		layout->addWidget(_descEdit, 2, 1);

		_loginLabel = new QLabel("Public login: ", clientSettingsWidget);
		_loginEdit = new QLineEdit(clientSettingsWidget);

		_isPublicButton = new QRadioButton("Public", clientSettingsWidget);
		if (dynamic_cast<GroupInfo*>(clientInfo)->isPublic())
		{
			_isPublicButton->setChecked(true);
			_loginEdit->setText(dynamic_cast<GroupInfo*>(clientInfo)->getPublicLogin());
		}
		else
		{
			_isPublicButton->setChecked(false);
			_loginLabel->hide();
			_loginEdit->hide();
		}
		connect(_isPublicButton, &QRadioButton::toggled, this, &ClientSettingsWindow::onPublicButtonToggled);

		layout->addWidget(_isPublicButton, 3, 0);
		layout->addWidget(_loginLabel, 4, 0);
		layout->addWidget(_loginEdit, 4, 1);
		layout->addWidget(showUsers, 5, 0);
		layout->addWidget(deleteThisGroup, 6, 0);
		layout->addWidget(apply, 7, 0);
		layout->addWidget(exit, 7, 1);
		layout->addWidget(_errorLabel, 8, 0);

        setWindowTitle("Group information");
    }
}

void ClientSettingsWindow::onExitButtonClicked()
{
    this->close();
}

void ClientSettingsWindow::onShowChannelUsersButtonClicked()
{
    Connector::getInstance()->loadChannelSubs(_info->getID());

    if(_listOfUsersWidget != nullptr)
    {
        delete _listOfUsersWidget;
        _map.clear();
    }
    _listOfUsersWidget = new QWidget(_stack);
    QVBoxLayout *layout = new QVBoxLayout(_listOfUsersWidget);

    QPushButton *backToSettings = new QPushButton("Back");
    QScrollArea *usersArea = new QScrollArea(_listOfUsersWidget);
    _usersAreaWidget = new QWidget(usersArea);
    _usersLayout = new QVBoxLayout(_usersAreaWidget);

    connect(backToSettings, &QPushButton::clicked, this, &ClientSettingsWindow::backToSettings);

    QList<UserInfo*> list = dynamic_cast<ChannelInfo*>(_info)->getListOfUsers();
    for(int i = 0; i != list.count(); i++)
    {
        UserLabel *userLabel = new UserLabel(list[i]->getID(), list[i]->getName(), _usersAreaWidget);
        _usersLayout->addWidget(userLabel);
        _map.insert(list[i]->getID(), userLabel);
        connect(userLabel, &UserLabel::clicked, this, &ClientSettingsWindow::onUserLabelClicked);
    }

    usersArea->setWidget(_usersAreaWidget);
    layout->addWidget(backToSettings);
    layout->addWidget(usersArea);

    _deleteSub = new QPushButton("Delete", _listOfUsersWidget);
    QPushButton *info = new QPushButton("Info", _listOfUsersWidget);

    layout->addWidget(info);
    layout->addWidget(_deleteSub);
    connect(info, &QPushButton::clicked, this, &ClientSettingsWindow::onInfoClicked);
    connect(_deleteSub, &QPushButton::clicked, this, &ClientSettingsWindow::onDeleteClicked);

	_login = new QLineEdit(_listOfUsersWidget);
	QPushButton *inviteButton = new QPushButton("Invite", _listOfUsersWidget);
	connect(inviteButton, &QPushButton::clicked, this, &ClientSettingsWindow::onInviteChannelClicked);
	layout->addWidget(_login);
	layout->addWidget(inviteButton);

	_errorLabelGroupUsers = new QLabel(this);
	_errorLabelGroupUsers->hide();

	layout->addWidget(_errorLabelGroupUsers);

    _stack->addWidget(_listOfUsersWidget);
    _stack->setCurrentWidget(_listOfUsersWidget);

    setWindowTitle("Channel subs");
}

void ClientSettingsWindow::onShowGroupUsersButtonClicked()
{
    //Connector::getInstance()->loadGroupUsers(_info->getID());

    if(_listOfUsersWidget != nullptr)
    {
        delete _listOfUsersWidget;
        _map.clear();
    }
    _listOfUsersWidget = new QWidget(_stack);
    QVBoxLayout *layout = new QVBoxLayout(_listOfUsersWidget);
    _listOfUsersWidget->setLayout(layout);

    layout->setAlignment(Qt::AlignTop);

    QPushButton *backToSettings = new QPushButton("Back");
    QScrollArea *usersArea = new QScrollArea(_listOfUsersWidget);
    _usersAreaWidget = new QWidget(usersArea);
    _usersLayout = new QVBoxLayout(_usersAreaWidget);

    connect(backToSettings, &QPushButton::clicked, this, &ClientSettingsWindow::backToSettings);

    QList<UserInfo*> list = dynamic_cast<GroupInfo*>(_info)->getListOfUser();
    for(int i = 0; i < list.count(); i++)
    {
        UserLabel *userLabel = new UserLabel(list[i]->getID(), list[i]->getName(), _usersAreaWidget);
        _usersLayout->addWidget(userLabel);
        _map.insert(list[i]->getID(), userLabel);
        connect(userLabel, &UserLabel::clicked, this, &ClientSettingsWindow::onUserLabelClicked);
    }

    usersArea->setWidget(_usersAreaWidget);
    layout->addWidget(backToSettings);
    layout->addWidget(usersArea);
    _usersAreaWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

    _deleteSub = new QPushButton("Delete", _listOfUsersWidget);
    QPushButton *info = new QPushButton("Info", _listOfUsersWidget);

    layout->addWidget(info);
    layout->addWidget(_deleteSub);
    connect(info, &QPushButton::clicked, this, &ClientSettingsWindow::onInfoClicked);
    connect(_deleteSub, &QPushButton::clicked, this, &ClientSettingsWindow::onDeleteClicked);

    QLabel *loginLabel = new QLabel("Input user login to invite:", _listOfUsersWidget);
    layout->addWidget(loginLabel);

    _errorLabelGroupUsers = new QLabel(this);
    _errorLabelGroupUsers->hide();

    _login = new QLineEdit(_listOfUsersWidget);
    QPushButton *inviteButton = new QPushButton("Invite", _listOfUsersWidget);
    connect(inviteButton, &QPushButton::clicked, this, &ClientSettingsWindow::onInviteClicked);
    layout->addWidget(_login);
    layout->addWidget(inviteButton);
    layout->addWidget(_errorLabelGroupUsers);

    _stack->addWidget(_listOfUsersWidget);
    _stack->setCurrentWidget(_listOfUsersWidget);

    setWindowTitle("Group users");
}

void ClientSettingsWindow::onUserLabelClicked(unsigned int id)
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
    if(id == Connector::getInstance()->getCurrentUserInfo()->getID())
        _deleteSub->hide();
    else
        _deleteSub->show();
    _currentID = id;
}

void ClientSettingsWindow::onDeleteClicked()
{
    if(_currentID != 0)
    {
        if(typeid (*_info) == typeid (ChannelInfo))
        {
            Connector::getInstance()->deleteSubFromChannel(_currentID, _info->getID());
            delete _map.find(_currentID).value();
        }
        else
        {
            Connector::getInstance()->deleteUserFromGroup(_currentID, _info->getID());
            delete _map.find(_currentID).value();
        }
    }
}

void ClientSettingsWindow::onInfoClicked()
{
    if(_currentID != 0)
    {
        Connector::getInstance()->loadUserInfo(_currentID);
        UserInfo *info = dynamic_cast<UserInfo*>(Connector::getInstance()->getUser(_currentID)->getInfo());
        ClientInfoWidnow *window = new ClientInfoWidnow(info, _listOfUsersWidget);
        window->exec();
    }
}

void ClientSettingsWindow::onInviteChannelClicked()
{
	UserInfo* info;
	if (Connector::getInstance()->getUser(_login->text()) != NULL)
	{
		info = dynamic_cast<UserInfo*>(Connector::getInstance()->getUser(_login->text())->getInfo());
		if (dynamic_cast<ChannelInfo*>(_info)->getListOfUsers().contains(info))
		{
			_errorLabelGroupUsers->setText("This user is already in channel");
			_errorLabelGroupUsers->show();
			return;
		}
	}
	Connector::getInstance()->inviteUserToChannel(_login->text(), _info->getID());
	if (Connector::getInstance()->getUser(_login->text()) != NULL)
	{
		info = dynamic_cast<UserInfo*>(Connector::getInstance()->getUser(_login->text())->getInfo());

		UserLabel *userLabel = new UserLabel(info->getID(), info->getName(), _usersAreaWidget);
		_usersLayout->addWidget(userLabel);
		_map.insert(info->getID(), userLabel);
		connect(userLabel, &UserLabel::clicked, this, &ClientSettingsWindow::onUserLabelClicked);

		_listOfUsersWidget->repaint();
		_errorLabelGroupUsers->hide();
	}
	else
	{
		_errorLabelGroupUsers->setText("Wrong login");
		_errorLabelGroupUsers->show();
	}
}

void ClientSettingsWindow::onInviteClicked()
{
    UserInfo* info;
    if(Connector::getInstance()->getUser(_login->text()) != NULL)
    {
        info = dynamic_cast<UserInfo*>(Connector::getInstance()->getUser(_login->text())->getInfo());
        if(dynamic_cast<GroupInfo*>(_info)->getListOfUser().contains(info))
        {
            _errorLabelGroupUsers->setText("This user is already in group");
            _errorLabelGroupUsers->show();
            return;
        }
    }
    Connector::getInstance()->inviteUserToGroup(_login->text(), _info->getID());
    if(Connector::getInstance()->getUser(_login->text()) != NULL)
    {
        info = dynamic_cast<UserInfo*>(Connector::getInstance()->getUser(_login->text())->getInfo());
        //dynamic_cast<GroupInfo*>(_info)->addNewUser(info);

        UserLabel *userLabel = new UserLabel(info->getID(), info->getName(), _usersAreaWidget);
        _usersLayout->addWidget(userLabel);
        _map.insert(info->getID(), userLabel);
        connect(userLabel, &UserLabel::clicked, this, &ClientSettingsWindow::onUserLabelClicked);

        _listOfUsersWidget->repaint();
        _errorLabelGroupUsers->hide();
    }
    else
    {
        _errorLabelGroupUsers->setText("Wrong login");
        _errorLabelGroupUsers->show();
    }
}

void ClientSettingsWindow::onApplyButtonClicked()
{
    QString name = _nameEdit->text();
    QString desc = _descEdit->text();
    bool isPublic = _isPublicButton->isChecked();
	QString login = _loginEdit->text();

    if(typeid (*_info) == typeid (ChannelInfo))
    {
		if (!(name.isEmpty() && desc.isEmpty()))
		{
			if (name.length() < 3 && name.length() > 0)
			{
				_errorLabel->setText("Name is too short");
				//_errorLabel->setPalette();
				_errorLabel->show();
			}
			else
			{
				advanChannelInfo info;
				if (isPublic)
				{
					if (login.length() < 3)
					{
						_errorLabel->setText("Public login is too short");
						_errorLabel->show();
						return;
					}
					info.publicLogin = login;
					dynamic_cast<ChannelInfo*>(_info)->setPublic(isPublic);
					dynamic_cast<ChannelInfo*>(_info)->setPublicLogin(login);
				}
				info.name = name;
				info.desc = desc;
				info.isPublic = isPublic;
				Connector::getInstance()->sendNewChannelInfo(_info->getID(), info);

				if (Connector::getInstance()->isErrorFromServer())
				{
					_errorLabel->setText("This login is already exists");
					_errorLabel->show();
					Connector::getInstance()->errorChecked();
					return;
				}

				_info->setName(name);
				_info->setDesc(desc);
				//dynamic_cast<ChannelInfo*>(_info)->setPublic(isPublic);
				_errorLabel->hide();
				onExitButtonClicked();
				emit changedInfo(_info->getID(), 2);
			}
		}
    }
    else
    {
		if (!(name.isEmpty() && desc.isEmpty()))
		{
			if (name.length() < 3 && name.length() > 0)
			{
				_errorLabel->setText("Name is too short");
				//_errorLabel->setPalette();
				_errorLabel->show();
			}
			else
			{
				advanGroupInfo info;
				if (isPublic)
				{
					if (login.length() < 3)
					{
						_errorLabel->setText("Public login is too short");
						_errorLabel->show();
						return;
					}
					info.publicLogin = login;
					dynamic_cast<GroupInfo*>(_info)->setPublicLogin(login);
				}
				info.name = name;
				info.desc = desc;
				info.isPublic = isPublic;
				Connector::getInstance()->sendNewGroupInfo(_info->getID(), info);
				if (Connector::getInstance()->isErrorFromServer())
				{
					_errorLabel->setText("This login is already exists");
					_errorLabel->show();
					Connector::getInstance()->errorChecked();
					return;
				}
				_info->setName(name);
				_info->setDesc(desc);
				dynamic_cast<GroupInfo*>(_info)->setPublic(isPublic);
				_errorLabel->hide();
				onExitButtonClicked();
				emit changedInfo(_info->getID(), 2);
			}
		}
    }
}

void ClientSettingsWindow::onDeleteClientClicked()
{
    if(_checkDeleteWindow == nullptr)
        delete _checkDeleteWindow;
    _checkDeleteWindow = new QWidget(clientSettingsWidget);
    QGridLayout *layout = new QGridLayout(_checkDeleteWindow);
    _checkDeleteWindow->setLayout(layout);

    QLabel *answer = new QLabel("Are you sure want to delete?", _checkDeleteWindow);
    QFont font;
    font.setPixelSize(20);
    answer->setAlignment(Qt::AlignCenter);
    answer->setFont(font);
    QPushButton *yes = new QPushButton("Yes", _checkDeleteWindow);
    if(typeid (ChannelInfo) == typeid (*_info))
        connect(yes, &QPushButton::clicked, this, &ClientSettingsWindow::onDeleteChannelClicked);
    else
        connect(yes, &QPushButton::clicked, this, &ClientSettingsWindow::onDeleteGroupClicked);

    QPushButton *no = new QPushButton("No", _checkDeleteWindow);
    connect(no, &QPushButton::clicked, this, &ClientSettingsWindow::backToSettings);

    layout->addWidget(answer, 0, 0, 1, 2);
    layout->addWidget(yes, 1, 0);
    layout->addWidget(no, 1, 1);
    _stack->addWidget(_checkDeleteWindow);
    _stack->setCurrentWidget(_checkDeleteWindow);
}

void ClientSettingsWindow::onDeleteChannelClicked()
{
	Connector::getInstance()->deleteChannel(_info->getID());
	unsigned int id = _info->getID();
	_info = nullptr;
	emit deletedClient(id, 2);
	this->reject();
}

void ClientSettingsWindow::onDeleteGroupClicked()
{
    Connector::getInstance()->deleteGroup(_info->getID());
	unsigned int id = _info->getID();
	_info = nullptr;
    emit deletedClient(id, 3);
    this->reject();
}

void ClientSettingsWindow::onPublicButtonToggled()
{
	if (_loginLabel->isHidden())
	{
		_loginLabel->show();
		_loginEdit->show();
	}
	else
	{
		_loginLabel->hide();
		_loginEdit->hide();
	}
}

void ClientSettingsWindow::backToSettings()
{
    _stack->setCurrentWidget(clientSettingsWidget);
}

ClientSettingsWindow::~ClientSettingsWindow()
{

}
