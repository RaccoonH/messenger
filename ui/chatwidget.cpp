#include "chatwidget.h"
#include <QDebug>

ChatWidget::ChatWidget(QWidget *parent) : QScrollArea(parent)
{
    _widget = new QWidget(this);
    _layout = new QVBoxLayout(_widget);

    _pall.setColor(QPalette::Window,Qt::white);
    setPalette(_pall);
    setAutoFillBackground(true);

    ///visual settings
    _layout->setAlignment(Qt::AlignBottom);
    _layout->setSpacing(2);
    _layout->setMargin(0);
    _layout->setDirection(QBoxLayout::BottomToTop);

    _layout->setSizeConstraint(QLayout::SetFixedSize);

    setWidget(_widget);
}

void ChatWidget::addNewMessage(Message *msg)
{
    MessageWidget *message = new MessageWidget(msg, this);
    _listOfMsgWidget.append(message);
    _layout->addWidget(message);
}

void ChatWidget::selectChat(Chat *chat)
{
    _chat = chat;
    clearChat();
    qDebug() << chat->getListOfMessages().count();
    for(int i = 0; i < chat->getListOfMessages().count(); i++)
    {
        addNewMessage(chat->getListOfMessages()[i]);
    }
}

void ChatWidget::clearChat()
{
    for(int i = 0; i < _listOfMsgWidget.count(); i++)
    {
        delete _listOfMsgWidget[i];
    }
    _listOfMsgWidget.clear();
}

ChatWidget::~ChatWidget()
{

}
