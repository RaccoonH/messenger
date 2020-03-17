#ifndef USER_H
#define USER_H

#include <QString>
#include "chat.h"
#include "userinfo.h"
#include "client.h"

class User : public Client
{
public:
    User(unsigned int id, UserInfo *userInfo, Chat *chat);
    ~User();

    ClientInfo* getInfo();
    Chat* getChat();

};

#endif // USER_H
