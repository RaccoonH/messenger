#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include "clientinfo.h"

class UserInfo : public ClientInfo
{
public:
    UserInfo(QString name, QString desc);
    QString getName();
    QString getDesc();
    void setName(QString name);
    void setDesc(QString desc);

};

#endif // USERINFO_H
