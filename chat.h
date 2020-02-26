#ifndef CHAT_H
#define CHAT_H

#include <QList>
#include "message.h"

class Chat
{
public:
    Chat();
    ~Chat();
    void addMessage(Message* msg);
    Message* getMessage();
    QList<Message*> getListOfMessages();

private:
    QList<Message*> _listOfMessages;

};

#endif // CHAT_H
