#include "groupinfo.h"

GroupInfo::GroupInfo(unsigned int id, QString name, QString desc, int countOfUsers, bool isPublic)
{
    _id = id;
    _name = name;
    _desc = desc;
    _countOfUsers = countOfUsers;
    _isPublic = isPublic;
}

void GroupInfo::setDesc(QString desc)
{
    _desc = desc;
}

void GroupInfo::setName(QString name)
{
    _name = name;
}

void GroupInfo::setID(unsigned int id)
{
    _id = id;
}

void GroupInfo::setCountOfUsers(int count)
{
    _countOfUsers = count;
}

void GroupInfo::setPublic(bool isPublic)
{
    _isPublic = isPublic;
}

void GroupInfo::setAdminName(QString name)
{
    _adminName = name;
}

void GroupInfo::setAdminID(int id)
{
    _adminID = id;
}

void GroupInfo::setPublicLogin(QString publicLogin)
{
	_publicLogin = publicLogin;
}

QString GroupInfo::getPublicLogin()
{
	return _publicLogin;
}

void GroupInfo::addNewUser(UserInfo *user)
{
    _listOfUsers.append(user);
}

void GroupInfo::deleteUser(unsigned int id)
{
    for(int i=0; i<_listOfUsers.count(); i++)
    {
        if(_listOfUsers[i]->getID() == id)
        {
            _listOfUsers.removeAt(i);
            break;
        }
    }
}

QString GroupInfo::getDesc()
{
    return _desc;
}

QString GroupInfo::getName()
{
    return _name;
}

unsigned int GroupInfo::getID()
{
    return _id;
}

int GroupInfo::getCountOfUsers()
{
    return _countOfUsers;
}

bool GroupInfo::isPublic()
{
    return _isPublic;
}

QString GroupInfo::getAdminName()
{
    return _adminName;
}

QList<UserInfo*> GroupInfo::getListOfUser()
{
    return _listOfUsers;
}

int GroupInfo::getAdminID()
{
    return _adminID;
}

GroupInfo::~GroupInfo()
{
    qDebug() << "GROUPINFO";
}
