#include "clientlist.h"
#include <QDebug>

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

    _pall.setColor(QPalette::Window,Qt::gray);
    setPalette(_pall);
    setAutoFillBackground(true);

//    _layout->addWidget(new QLabel("123", this));
//    _layout->addWidget(new QLabel("123", this));

   // _layout->setSpacing(110);

    setWidget(_clientListWidget);
}

void ClientList::addNewClient(unsigned int id, UserInfo *userInfo, Chat *chat)
{
    ClientLabel *label = new ClientLabel(id, userInfo, chat, _clientListWidget);
    _layout->addWidget(label);
    emit addedNewClient(label);
}

ClientList::~ClientList()
{

}
