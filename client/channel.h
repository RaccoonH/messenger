#ifndef CHANNEL_H
#define CHANNEL_H

#include "client.h"
#include "channelinfo.h"

class Channel : public Client
{
public:
    Channel(ChannelInfo *channelInfo = 0, Chat *chat = 0);
    ~Channel();

    ClientInfo* getInfo();
    Chat* getChat();
    void setChat(Chat*);

private:

};

#endif // CHANNEL_H
