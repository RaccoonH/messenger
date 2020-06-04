#ifndef GROUPINFO_H
#define GROUPINFO_H

#include <QList>
#include "clientinfo.h"
#include "userinfo.h"

class GroupInfo : public ClientInfo
{
public:
    GroupInfo(unsigned int id, QString name = 0, QString desc = 0, int countOfUsers = 0, bool isPublic = true);
    ~GroupInfo();

    QString getName();
    QString getDesc();
    unsigned int getID();
    int getCountOfUsers();
    int getAdminID();
    QString getAdminName();
    QList<UserInfo*> getListOfUser();
    bool isPublic();
	QString getPublicLogin();

    void setName(QString name);
    void setDesc(QString desc);
    void setID(unsigned int id);
    void setCountOfUsers(int count);
    void setAdminID(int id);
    void setAdminName(QString name);
    void addNewUser(UserInfo* name);
    void setPublic(bool isPublic);
	void setPublicLogin(QString publicLogin);

    void deleteUser(unsigned int id);

private:
    int _countOfUsers;
    bool _isPublic;
    QString _adminName;
    int _adminID;
    QList<UserInfo*> _listOfUsers;
	QString _publicLogin;
    //mb need later
    //QList<UserInfo*> _listOfUsersInfo;

};

#endif // GROUPINFO_H
