#include "messenger_window.h"
#include <QDebug>

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


    connect(_clientList, &ClientList::addedNewClient, this, &MessengerWindow::addNewChat);

    ///test Create User Info
    for(int i = 0; i < 10; i++)
    {
        UserInfo *userInfo = new UserInfo("testName", "testDesc");
        Chat *chat = new Chat();
        for(int j = 0; j < 2; j++)
        {
            chat->addMessage(new Message(QString::number(i) + QString::number(j) + " 1488ss"));
        }
        _clientList->addNewClient(i, userInfo, chat);
    }

}

void MessengerWindow::addNewChat(ClientLabel *label)
{
    qDebug() << "check";
    connect(label, &ClientLabel::clicked, _chatDisplay, &ChatDisplay::addNewChat);
}

MessengerWindow::~MessengerWindow()
{

}
