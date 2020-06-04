#ifndef CLIENT_H
#define CLIENT_H

#include "chat.h"
#include "clientinfo.h"

class Client
{
public:
    Client();

    virtual Chat* getChat() = 0;
    virtual ClientInfo* getInfo() = 0;
    virtual void setChat(Chat*) = 0;

    virtual ~Client() = 0;

protected:
    Chat *_chat;
    ClientInfo *_info;

};

#endif // CLIENT_H
