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
    _layout->setDirection(QBoxLayout::TopToBottom);

    _layout->setSizeConstraint(QLayout::SetFixedSize);

    ensureVisible(0,0);

    setWidget(_widget);
}

Chat* ChatWidget::getChat()
{
    return _chat;
}

void ChatWidget::addNewMessage(Message *msg)
{
    MessageWidget *messageWidget = new MessageWidget(msg, this);
    _listOfMsgWidget.append(messageWidget);
	connect(messageWidget, &MessageWidget::clicked, this, &ChatWidget::onMessageWidgetClicked);
    _layout->addWidget(messageWidget);
}

void ChatWidget::selectChat(Chat *chat)
{
    _chat = chat;
    clearChat();
    for(int i = 0; i < chat->getListOfMessages().count(); i++)
    {
        addNewMessage(chat->getListOfMessages()[i]);
    }
}

void ChatWidget::deleteMessageWidget(MessageWidget *pointer)
{
	_listOfMsgWidget.removeOne(pointer);
	delete pointer;
}

void ChatWidget::deleteMessageWidget(unsigned int id, QDateTime date)
{
	clearChat();
	for (int i = 0; i < _chat->getListOfMessages().count(); i++)
	{
		addNewMessage(_chat->getListOfMessages()[i]);
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

void ChatWidget::onMessageWidgetClicked(Message* msg, MessageWidget *pointer)
{
	emit messageWidgetClicked(msg, pointer);
}

void ChatWidget::editMessageWidget(MessageWidget *pointer)
{
	pointer->updateNewText();
}

void ChatWidget::editMessageWidget(Message *msg)
{
	for (int i = 0; i < _listOfMsgWidget.count(); i++)
	{
		if (_listOfMsgWidget[i]->getMessage() == msg)
		{
			_listOfMsgWidget[i]->updateNewText();
		}
	}
}

ChatWidget::~ChatWidget()
{

}
