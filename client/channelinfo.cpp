#include "channelinfo.h"

ChannelInfo::ChannelInfo(unsigned int id, QString name, QString desc, int countOfSubs, bool isPublic, QString publicLogin)
{
    _id = id;
    _name = name;
    _desc = desc;
    _countOfSubs = countOfSubs;
    _isPublic = isPublic;
	if (_isPublic)
		_publicLogin = publicLogin;
}

void ChannelInfo::setDesc(QString desc)
{
    _desc = desc;
}

void ChannelInfo::setName(QString name)
{
    _name = name;
}

void ChannelInfo::setID(unsigned int id)
{
    _id = id;
}

void ChannelInfo::setCountOfSubs(int count)
{
    _countOfSubs = count;
}

void ChannelInfo::setPublic(bool isPublic)
{
    _isPublic = isPublic;
}

QString ChannelInfo::getDesc()
{
    return _desc;
}

QString ChannelInfo::getName()
{
    return _name;
}

unsigned int ChannelInfo::getID()
{
    return _id;
}

int ChannelInfo::getCountOfSubs()
{
    return _countOfSubs;
}

bool ChannelInfo::isPublic()
{
    return _isPublic;
}

void ChannelInfo::setPublicLogin(QString publicLogin)
{
	_publicLogin = publicLogin;
}

QString ChannelInfo::getPublicLogin()
{
	return _publicLogin;
}

void ChannelInfo::addNewUser(UserInfo* info)
{
    _listOfUsers.append(info);
}

void ChannelInfo::deleteUser(unsigned int id)
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

QList<UserInfo*> ChannelInfo::getListOfUsers()
{
    return _listOfUsers;
}

ChannelInfo::~ChannelInfo()
{
    qDebug() << "CHANNELINFO";
}
