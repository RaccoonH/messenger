#ifndef USER_H
#define USER_H

#include <QString>
#include "chat.h"
#include "userinfo.h"
#include "client.h"

class User : public Client
{
public:
    User(UserInfo *userInfo = 0, Chat *chat = 0);
    ~User();

    ClientInfo* getInfo();
    Chat* getChat();
    void setChat(Chat*);

};

#endif // USER_H
