#ifndef USER_H
#define USER_H

#include <QString>
#include "chat.h"
#include "userinfo.h"

class User
{
public:
    User(unsigned int id, UserInfo *userInfo, Chat *chat);
    ~User();

    UserInfo* getUserInfo();
    Chat* getChat();

private:
    unsigned int _id;
    UserInfo *_userInfo;
    Chat *_chat;

};

#endif // USER_H
