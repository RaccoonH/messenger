#include "messenger_window.h"

MessengerWindow::MessengerWindow(QWidget *parent) : QWidget(parent)
{
    _messengerWindowLayout = new QHBoxLayout(this);
    setLayout(_messengerWindowLayout);

    _clientList = new ClientList(this);
    _chatDisplay = new ChatDisplay(this);

    _messengerWindowLayout->setSpacing(0);
    _messengerWindowLayout->setMargin(0);
    _messengerWindowLayout->addWidget(_clientList);
    _messengerWindowLayout->addWidget(_chatDisplay);

    //connect(Connector::getInstance(), &Connector::loadedClientList, this, &MessengerWindow::onLoadedClientList);
    connect(_clientList, &ClientList::addedNewClient, this, &MessengerWindow::onAddedNewClient);
    connect(_chatDisplay, &ChatDisplay::changedInfo, _clientList, &ClientList::onChangedInfo);
    connect(_chatDisplay, &ChatDisplay::deletedClient, _clientList, &ClientList::onDeletedClient);
    //connect(Connector::getInstance(), &Connector::loadedDialogChat, _chatDisplay, &ChatDisplay::onLoadedDialogChat);
    //connect(Connector::getInstance(), &Connector::loadedGroupChat, _chatDisplay, &ChatDisplay::onLoadedGroupChat);
    //connect(Connector::getInstance(), &Connector::loadedChannelChat, _chatDisplay, &ChatDisplay::onLoadedChannelChat);

    connect(Connector::getInstance(), &Connector::loadedNewDialogMessage, _chatDisplay, &ChatDisplay::onLoadedNewDialogMessage);
    connect(Connector::getInstance(), &Connector::loadedNewGroupMessage, _chatDisplay, &ChatDisplay::onLoadedNewGroupMessage);
    connect(Connector::getInstance(), &Connector::loadedNewChannelMessage, _chatDisplay, &ChatDisplay::onLoadedNewChannelMessage);

    connect(Connector::getInstance(), &Connector::loadedNewClientInfo, _clientList, &ClientList::onChangedInfo);
	connect(Connector::getInstance(), &Connector::deletedFromClient, _clientList, &ClientList::onDeletedClient);
	connect(Connector::getInstance(), &Connector::deletedClient, _clientList, &ClientList::onDeletedClient);
	connect(Connector::getInstance(), &Connector::deletedMessage, _chatDisplay, &ChatDisplay::onDeletedMessage);
	connect(Connector::getInstance(), &Connector::changedMessage, _chatDisplay, &ChatDisplay::onChangedMessage);
	connect(Connector::getInstance(), &Connector::addedByClient, _clientList, &ClientList::addNewClient);
//    connect(Connector::getInstance(), &Connector::loadedChannelInfo, _chatDisplay, &ChatDisplay::onLoadedChannelInfo);
//    connect(Connector::getInstance(), &Connector::loadedGroupInfo, _chatDisplay, &ChatDisplay::onLoadedGroupInfo);

	Connector::getInstance()->loadClientList();

	for (int i = 0; i < Connector::getInstance()->getListOfClient().size(); ++i)
	{
		_clientList->addNewClient(Connector::getInstance()->getListOfClient()[i]->getInfo()->getID(),
			Connector::getInstance()->getListOfClient()[i]->getInfo());
	}

	Connector::getInstance()->loadListOfFriends();

}

void MessengerWindow::onAddedNewClient(ClientLabel *label)
{
    connect(label, &ClientLabel::clicked, _chatDisplay, &ChatDisplay::onClientLabelClicked);
}

void MessengerWindow::onLoadedClientList()
{
	for (int i = 0; i < Connector::getInstance()->getListOfClient().size(); ++i)
	{
		_clientList->addNewClient(Connector::getInstance()->getListOfClient()[i]->getInfo()->getID(),
			Connector::getInstance()->getListOfClient()[i]->getInfo());
	}
}

MessengerWindow::~MessengerWindow()
{

}
