#include "clientlist.h"
#include <QDebug>
#include <QThread>

ClientList::ClientList(QWidget *parent) : QScrollArea(parent)
{
    setMinimumWidth(WIDTH_CLIENTLIST);
    setMaximumWidth(WIDTH_CLIENTLIST);
    _clientListWidget = new QWidget(this);
    _layout = new QVBoxLayout(_clientListWidget);

    ///visual settings
    _layout->setAlignment(Qt::AlignTop);
    _layout->setSpacing(2);
    _layout->setMargin(0);
    _layout->setSizeConstraint(QLayout::SetFixedSize);
	setWidgetResizable(true);

    _pall.setColor(QPalette::Window,Qt::gray);
    setPalette(_pall);
    setAutoFillBackground(true);

    _settingsButton = new QPushButton("Settings", this);
    _layout->addWidget(_settingsButton);
    connect(_settingsButton, &QPushButton::clicked, this, &ClientList::onSettingsButtonClicked);
    setWidget(_clientListWidget);
}

void ClientList::addNewClient(unsigned int id, ClientInfo *clientInfo)
{
    ClientLabel *label = new ClientLabel(id, clientInfo, _clientListWidget);
    connect(label, &ClientLabel::clicked, this, &ClientList::onClientLabelClicked);
    _layout->addWidget(label);
    _list.append(label);
    emit addedNewClient(label);
}

void ClientList::onClientLabelClicked(unsigned int id, int type)
{
    for(int i = 0; i < _list.count(); i++)
    {
        if(_list[i]->getID() == id && _list[i]->getType() == type)
        {
            _list[i]->fillLabel();
        }
        else
        {
            _list[i]->unfillLabel();
        }
    }
}

void ClientList::onChangedInfo(unsigned int id, int type)
{
    for(int i = 0; i < _list.count(); i++)
    {
        if(_list[i]->getID() == id && _list[i]->getType() == type)
        {
            _list[i]->updateInfo();
        }
    }
}

void ClientList::onDeletedClient(unsigned int id, int type)
{
    for(int i = 0; i < _list.count(); i++)
    {
        if(_list[i]->getID() == id && _list[i]->getType() == type)
        {
            delete _list[i];
            _list.removeAt(i);
        }
    }
}

void ClientList::onSettingsButtonClicked()
{
	SettingsWindow sWindow(this);
    connect(&sWindow, &SettingsWindow::createdNewChannel, this, &ClientList::addNewClient);
	connect(&sWindow, &SettingsWindow::addedNewClient, this, &ClientList::addNewClient);
    sWindow.exec();	
}

ClientList::~ClientList()
{

}
