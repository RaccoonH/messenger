#include "group.h"

Group::Group(GroupInfo *groupInfo, Chat *chat)
{
    _info = groupInfo;
    _chat = chat;
}

ClientInfo* Group::getInfo()
{
    return _info;
}

Chat* Group::getChat()
{
    return _chat;
}

void Group::setChat(Chat *chat)
{
    _chat = chat;
}

Group::~Group()
{

}
