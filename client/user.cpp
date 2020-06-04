#include "user.h"

User::User(UserInfo *userInfo, Chat *chat)
{
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

void User::setChat(Chat *chat)
{
    _chat = chat;
}

User::~User()
{

}
