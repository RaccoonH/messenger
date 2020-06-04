#include "chatdisplay.h"

ChatDisplay::ChatDisplay(QWidget *parent) : QWidget(parent)
{
    _layout = new QVBoxLayout(this);

    _headLine = createHeadline(this);
    _chatWidget = new ChatWidget(this);
    _entryField = createEntryField(this);

	QAction *edit = _menu.addAction("Edit");
	QAction *del =_menu.addAction("Delete");
	connect(del, &QAction::triggered, this, &ChatDisplay::onDeleteMessageClicked);
	connect(edit, &QAction::triggered, this, &ChatDisplay::onEditMessageClicked);

	connect(_chatWidget, &ChatWidget::messageWidgetClicked, this, &ChatDisplay::onMessageWidgetClicked);

//    _pall.setColor(QPalette::Window,Qt::gray);
//    setPalette(_pall);
//    setAutoFillBackground(true);

    _layout->setMargin(0);

    _layout->addWidget(_headLine);
    _layout->addWidget(_chatWidget);
    _layout->addWidget(_entryField);
}

QWidget* ChatDisplay::createHeadline(QWidget* parent)
{
    QWidget *headline = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(headline);

    _userNameLabel = new QLabel("name", headline);
    ///this button is test
    QPushButton *clientInfoButton = new QPushButton("Info", headline);
    connect(clientInfoButton, &QPushButton::clicked, this, &ChatDisplay::onClientInfoButtonClicked);

    _clientSettings = new QPushButton("Settings", headline);
    _clientSettings->hide();
    connect(_clientSettings, &QPushButton::clicked, this, &ChatDisplay::onSettingsClicked);

    layout->addWidget(_userNameLabel);
    layout->addWidget(clientInfoButton);
    layout->addWidget(_clientSettings);

    headline->setMaximumHeight(MAX_HEIGHT_HEADLINE);
    return headline;
}

QWidget* ChatDisplay::createEntryField(QWidget *parent)
{
    QWidget *entryField = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(entryField);
    _textEdit = new QTextEdit(entryField);

    _sendButton = new QPushButton("Send", entryField);
    connect(_sendButton, &QPushButton::clicked, this, &ChatDisplay::onSendButtonClicked);

    layout->addWidget(_textEdit);
    layout->addWidget(_sendButton);

    entryField->setMaximumHeight(MAX_HEIGHT_ENTRYFIELD);
    return entryField;
}

void ChatDisplay::onClientLabelClicked(unsigned int id, int clientType)
{
    //qDebug() << Connector::getInstance()->getUser(id)->getChat();
    _currentClientID = id;
    _currentClientType = clientType;
    qDebug() << "onClientLabelClicked";
    switch (clientType)
    {
    case 1:
    {
        if(Connector::getInstance()->getUser(id)->getChat() == NULL)
        {
            Connector::getInstance()->loadDialogMessages(id);
			_chatWidget->selectChat(Connector::getInstance()->getUser(id)->getChat());
            qDebug() << "getchat";
        }
        else
        {
            _chatWidget->selectChat(Connector::getInstance()->getUser(id)->getChat());
        }

        if(_sendButton->isHidden())
        {
            _textEdit->show();
            _sendButton->show();
        }

        _clientSettings->hide();
        _userNameLabel->setText(Connector::getInstance()->getUser(id)->getInfo()->getName());

        break;
    }

    case 2:
    {
        if(Connector::getInstance()->getChannel(id)->getChat() == NULL)
        {
            Connector::getInstance()->loadChannelMessages(id);
			_chatWidget->selectChat(Connector::getInstance()->getChannel(id)->getChat());
        }
        else
        {
            _chatWidget->selectChat(Connector::getInstance()->getChannel(id)->getChat());

        }

        if(!Connector::getInstance()->isAdminOfChannel(id))
        {
            _textEdit->hide();
            _sendButton->hide();
            _clientSettings->hide();
        }
        else
        {
            _clientSettings->show();
            if(_sendButton->isHidden())
            {
                _textEdit->show();
                _sendButton->show();
            }
        }
        _userNameLabel->setText(Connector::getInstance()->getChannel(id)->getInfo()->getName());
        break;
    }

    case 3:
    {
        if(Connector::getInstance()->getGroup(id)->getChat() == NULL)
        {
            Connector::getInstance()->loadGroupMessages(id);
			_chatWidget->selectChat(Connector::getInstance()->getGroup(id)->getChat());

        }
        else
        {
            _chatWidget->selectChat(Connector::getInstance()->getGroup(id)->getChat());

        }

        if(Connector::getInstance()->isAdminOfGroup(id))
            _clientSettings->show();
        else
            _clientSettings->hide();

        if(_sendButton->isHidden())
        {
            _textEdit->show();
            _sendButton->show();
        }
        _userNameLabel->setText(Connector::getInstance()->getGroup(id)->getInfo()->getName());
        break;
    }
    }
}

//void ChatDisplay::onLoadedDialogChat(unsigned int id)
//{
//    _chatWidget->selectChat(Connector::getInstance()->getUser(id)->getChat());
//}
//
//void ChatDisplay::onLoadedGroupChat(unsigned int id)
//{
//    _chatWidget->selectChat(Connector::getInstance()->getGroup(id)->getChat());
//}
//
//void ChatDisplay::onLoadedChannelChat(unsigned int id)
//{
//    _chatWidget->selectChat(Connector::getInstance()->getChannel(id)->getChat());
//}

void ChatDisplay::onSendButtonClicked()
{
    switch (_currentClientType)
    {
    case 1:
    {
        Connector::getInstance()->sendDialogMessage(_currentClientID, _textEdit->toPlainText(), QDateTime::currentDateTime());
        break;
    }
    case 2:
    {
        Connector::getInstance()->sendChannelMessage(_currentClientID, _textEdit->toPlainText(), QDateTime::currentDateTime());
        break;
    }
    case 3:
    {
        Connector::getInstance()->sendGroupMessage(_currentClientID, _textEdit->toPlainText(), QDateTime::currentDateTime());
        break;
    }
    }

}

void ChatDisplay::onClientInfoButtonClicked()
{
	ClientInfoWidnow *window;
    switch (_currentClientType)
    {
	case 0:
		return;
    case 1:
    {
        Connector::getInstance()->loadUserInfo(_currentClientID);
        window = new ClientInfoWidnow(Connector::getInstance()->getUser(_currentClientID)->getInfo(), this);
        break;
    }
    case 2:
    {
        Connector::getInstance()->loadChannelInfo(_currentClientID);
		window = new ClientInfoWidnow(Connector::getInstance()->getChannel(_currentClientID)->getInfo(), this);
        break;
    }
    case 3:
    {
        Connector::getInstance()->loadGroupInfo(_currentClientID);
		window = new ClientInfoWidnow(Connector::getInstance()->getGroup(_currentClientID)->getInfo(), this);
        break;
    }
    }
	window->setAttribute(Qt::WA_DeleteOnClose);
	connect(window, &ClientInfoWidnow::deletedClient, this, &ChatDisplay::onDeletedClient);
	window->exec();
}

void ChatDisplay::onLoadedNewDialogMessage(unsigned int id)
{
    qDebug() << "loadedNewDIALOGmsg";
    if((_currentClientID == id)&&(_currentClientType == 1))
    {
        _chatWidget->selectChat(Connector::getInstance()->getUser(id)->getChat());
    }
    else
    {

    }
}

void ChatDisplay::onLoadedNewChannelMessage(unsigned int id)
{
    qDebug() << "loadedNewCHANNELRmsg";
    if((_currentClientID == id)&&(_currentClientType == 2))
    {
        _chatWidget->selectChat(Connector::getInstance()->getChannel(id)->getChat());
    }
    else
    {

    }
}

void ChatDisplay::onLoadedNewGroupMessage(unsigned int id)
{
    qDebug() << "loadedNewGROYPRmsg";
    if((_currentClientID == id)&&(_currentClientType == 3))
    {
        _chatWidget->selectChat(Connector::getInstance()->getGroup(id)->getChat());
    }
    else
    {

    }
}

void ChatDisplay::onSettingsClicked(unsigned int id)
{
    ClientSettingsWindow *window;
    switch (_currentClientType)
    {
    case 2:
    {
		Connector::getInstance()->loadChannelInfo(_currentClientID);
        window = new ClientSettingsWindow(Connector::getInstance()->getChannel(_currentClientID)->getInfo(), this);
        break;
    }
    case 3:
    {
		Connector::getInstance()->loadGroupInfo(_currentClientID);
        window = new ClientSettingsWindow(Connector::getInstance()->getGroup(_currentClientID)->getInfo(), this);
        break;
    }
    }
    connect(window, &ClientSettingsWindow::changedInfo, this, &ChatDisplay::onChangedInfo);
    connect(window, &ClientSettingsWindow::deletedClient, this, &ChatDisplay::onDeletedClient);
    window->exec();
}

void ChatDisplay::onChangedInfo(unsigned int id, int type)
{
    emit changedInfo(id, type);
}

void ChatDisplay::onDeletedClient(unsigned int id, int type)
{
    emit deletedClient(id, type);
}

void ChatDisplay::onMessageWidgetClicked(Message *msg, MessageWidget *pointer)
{
	_msgWidgetPointer = pointer;
	_msg = msg;
	switch (_currentClientType)
	{
	case 1:
	{
		if (Connector::getInstance()->getCurrentUserInfo()->getID() == msg->getSenderID())
		{
			_menu.exec(QCursor::pos());
		}
		break;
	}
	case 2:
	{
		if (Connector::getInstance()->isAdminOfChannel(msg->getSenderID()))
		{
			_menu.exec(QCursor::pos());
		}
		break;
	}
	case 3:
	{
		if (Connector::getInstance()->getCurrentUserInfo()->getID() == msg->getSenderID())
		{
			_menu.exec(QCursor::pos());
		}
	}
	}
}

void ChatDisplay::onDeleteMessageClicked()
{
	Connector::getInstance()->deleteMessage(_msg, _currentClientType, _currentClientID);

	switch (_currentClientType)
	{
	case 1:
	{
		Connector::getInstance()->getUser(_currentClientID)->getChat()->deleteMessage(_msg);
		break;
	}
	case 2:
	{
		Connector::getInstance()->getChannel(_currentClientID)->getChat()->deleteMessage(_msg);
		break;
	}
	case 3:
	{
		Connector::getInstance()->getGroup(_currentClientID)->getChat()->deleteMessage(_msg);
		break;
	}
	}
	_msg = nullptr;
	_chatWidget->deleteMessageWidget(_msgWidgetPointer);

}

void ChatDisplay::onEditMessageClicked()
{
	QDialog edit;
	QVBoxLayout layout;
	QLabel label("Edit");
	_lineEditMsg = new QLineEdit(&edit);
	_lineEditMsg->setText(_msg->getText());
	QPushButton button("Apply");
	connect(&button, &QPushButton::clicked, this, &ChatDisplay::onApplyEditMessageClicked);
	connect(&button, &QPushButton::clicked, &edit, &QDialog::reject);
	layout.addWidget(&label);
	layout.addWidget(_lineEditMsg);
	layout.addWidget(&button);
	edit.setLayout(&layout);
	edit.exec();
}

void ChatDisplay::onApplyEditMessageClicked()
{
	Connector::getInstance()->editMessage(_msg, _lineEditMsg->text(), _currentClientType, _currentClientID);

	_msg->setText(_lineEditMsg->text());
	_chatWidget->editMessageWidget(_msgWidgetPointer);


}

void ChatDisplay::onDeletedMessage(unsigned int id, QDateTime date)
{
	_chatWidget->deleteMessageWidget(id, date);
}

void ChatDisplay::onChangedMessage(Message *msg)
{
	_chatWidget->editMessageWidget(msg);
}

//
//void ChatDisplay::onLoadedChannelInfo(unsigned int id)
//{
//    ClientInfoWidnow *window = new ClientInfoWidnow(Connector::getInstance()->getChannel(id)->getInfo(), this);
//    window->exec();
//}
//
//void ChatDisplay::onLoadedGroupInfo(unsigned int id)
//{
//    ClientInfoWidnow window(Connector::getInstance()->getGroup(id)->getInfo(), this);
//    window->exec();
//}

void ChatDisplay::addNewChat(Chat *chat)
{
    _chatWidget->selectChat(chat);
}

ChatDisplay::~ChatDisplay()
{

}
