#ifndef GROUP_H
#define GROUP_H

#include "client.h"
#include "groupinfo.h"

class Group : public Client
{
public:
    Group(GroupInfo *groupInfo = 0, Chat *chat = 0);
    ~Group();

    ClientInfo* getInfo();
    Chat* getChat();
    void setChat(Chat*);

private:

};

#endif // GROUP_H
