#include "userinfo.h"

UserInfo::UserInfo(unsigned int id, QString name, QString login, QString desc, QString number)
{
    _id = id;
    _name = name;
    _login = login;
    _desc = desc;
    _number = number;
}

void UserInfo::setDesc(QString desc)
{
    _desc = desc;
}

void UserInfo::setName(QString name)
{
    _name = name;
}

void UserInfo::setLogin(QString login)
{
    _login = login;
}

void UserInfo::setID(unsigned int id)
{
    _id = id;
}

void UserInfo::setNubmer(QString number)
{
    _number = number;
}

QString UserInfo::getDesc()
{
    return _desc;
}

QString UserInfo::getName()
{
    return _name;
}

QString UserInfo::getLogin()
{
    return _login;
}

unsigned int UserInfo::getID()
{
    return _id;
}

QString UserInfo::getNumber()
{
    return _number;
}

UserInfo::~UserInfo()
{
    qDebug() << "USERINFO";
}
