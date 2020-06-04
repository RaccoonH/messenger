#include "chat.h"

Chat::Chat()
{

}

void Chat::addMessage(Message *msg)
{
    _listOfMessages.append(msg);
}

QList<Message*> Chat::getListOfMessages()
{
    return _listOfMessages;
}

int Chat::getType()
{
    return _clientType;
}

void Chat::deleteMessage(Message *msg)
{
	_listOfMessages.removeOne(msg);
	delete msg;
}

void Chat::deleteMessage(unsigned int senderID, QDateTime date)
{
	for (int i = 0; i < _listOfMessages.count(); i++)
	{
		if (_listOfMessages[i]->getSenderID() == senderID && _listOfMessages[i]->getTime() == date)
		{
			delete _listOfMessages[i];
			_listOfMessages[i] = NULL;
			_listOfMessages.removeAt(i);
			break;
		}
	}
}

Message* Chat::getMessage(unsigned int senderID, QDateTime date)
{
	for (int i = 0; i < _listOfMessages.count(); i++)
	{
		if (_listOfMessages[i]->getSenderID() == senderID && _listOfMessages[i]->getTime() == date)
		{
			return _listOfMessages[i];
		}
	}
}

Chat::~Chat()
{
	for (int i = 0; i < _listOfMessages.count(); i++)
	{
		delete _listOfMessages[i];
	}
}
