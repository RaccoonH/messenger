#include "user.h"
#include <QDebug>

User::User(unsigned int id, UserInfo *userInfo, Chat *chat)
{
    _id = id;
    _info = userInfo;
    _chat = chat;
}

ClientInfo* User::getInfo()
{
    return _info;
}

Chat* User::getChat()
{
    return _chat;
}

User::~User()
{
    delete  _info;
    delete _chat;
}
