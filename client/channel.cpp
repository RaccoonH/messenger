#include "channel.h"

Channel::Channel(ChannelInfo *channelInfo, Chat *chat)
{
    _info = channelInfo;
    _chat = chat;
}

ClientInfo* Channel::getInfo()
{
    return _info;
}

Chat* Channel::getChat()
{
    return _chat;
}

void Channel::setChat(Chat *chat)
{
    _chat = chat;
}

Channel::~Channel()
{

}
