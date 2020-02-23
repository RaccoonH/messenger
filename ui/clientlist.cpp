#include "clientlist.h"

ClientList::ClientList(QWidget *parent) : QScrollArea(parent)
{
    setMinimumWidth(WIDTH_CLIENTLIST);
    setMaximumWidth(WIDTH_CLIENTLIST);
    _clientListWidget = new QWidget(this);
    _layout = new QVBoxLayout(_clientListWidget);

    _pall.setColor(QPalette::Window,Qt::gray);
    setPalette(_pall);
    setAutoFillBackground(true);

    for(int i = 0; i < 5; i++)
    {
        addNewClient("123");
    }
    setWidget(_clientListWidget);

}

void ClientList::addNewClient(QString name)
{
    ClientLabel *label = new ClientLabel(_clientListWidget);
    _layout->addWidget(label);
}

ClientList::~ClientList()
{

}
