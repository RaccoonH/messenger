#ifndef CHAT_H
#define CHAT_H

#include <QList>
#include "message.h"
#include "data_struct.h"

class Chat
{
public:
    Chat();
    ~Chat();

    void addMessage(Message* msg);
    QList<Message*> getListOfMessages();
    int getType();
	void deleteMessage(Message *msg);
	void deleteMessage(unsigned int senderID, QDateTime date);
	Message* getMessage(unsigned int senderID, QDateTime date);

private:
    QList<Message*> _listOfMessages;
    int _clientType;

};

#endif // CHAT_H
