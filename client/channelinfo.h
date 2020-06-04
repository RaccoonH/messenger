#ifndef CHANNELINFO_H
#define CHANNELINFO_H

#include <QList>
#include "clientinfo.h"
#include "userinfo.h"

class ChannelInfo : public ClientInfo
{
public:
    ChannelInfo(unsigned int id, QString name = 0, QString desc = 0, int countOfSubs = 0, bool isPublic = 0, QString publicName = 0);
    ~ChannelInfo();

    QString getName();
    QString getDesc();
    unsigned int getID();
    int getCountOfSubs();
    bool isPublic();
	QString getPublicLogin();

    void setName(QString name);
    void setDesc(QString desc);
    void setID(unsigned int id);
    void setCountOfSubs(int count);
    void setPublic(bool isPublic);
	void setPublicLogin(QString publicLogin);

    void addNewUser(UserInfo*);
    void deleteUser(unsigned int id);
    QList<UserInfo*> getListOfUsers();

private:
    QList<UserInfo*> _listOfUsers;
    int _countOfSubs;
    bool _isPublic;
	QString _publicLogin;
};

#endif // CHANNELINFO_H
