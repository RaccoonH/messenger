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
}

MessengerWindow::~MessengerWindow()
{

}
