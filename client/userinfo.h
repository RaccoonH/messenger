#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include "clientinfo.h"
#include <QDebug>

class UserInfo : public ClientInfo
{
public:
    UserInfo(unsigned int id, QString name = 0, QString login = 0, QString desc = 0, QString number = 0);
    ~UserInfo();
    QString getName();
    QString getLogin();
    QString getDesc();
    unsigned int getID();
    QString getNumber();

    void setName(QString name);
    void setLogin(QString login);
    void setDesc(QString desc);
    void setID(unsigned int id);
    void setNubmer(QString number);

private:
    QString _login;
    QString _number;
};

#endif // USERINFO_H
