#include "userinfo.h"

UserInfo::UserInfo(QString name, QString desc)
{
    _name = name;
    _desc = desc;
}



void UserInfo::setDesc(QString desc)
{
    _desc = desc;
}

void UserInfo::setName(QString name)
{
    _name = name;
}

QString UserInfo::getDesc()
{
    return _desc;
}

QString UserInfo::getName()
{
    return _name;
}
