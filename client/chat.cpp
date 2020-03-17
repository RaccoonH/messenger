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

Chat::~Chat()
{
    while(!_listOfMessages.isEmpty())
    {
        delete _listOfMessages.takeFirst();
    }
}
