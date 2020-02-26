#include "user.h"
#include <QDebug>

User::User(unsigned int id, UserInfo *userInfo, Chat *chat)
{
    _id = id;
    _userInfo = userInfo;
    _chat = chat;
}

UserInfo* User::getUserInfo()
{
    return _userInfo;
}

Chat* User::getChat()
{
    return _chat;
}

User::~User()
{
    delete  _userInfo;
    delete _chat;
}
