#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <QString>
#include <QDateTime>

struct baseClientInfo
{
    int type;
    unsigned int id;
    QString name;
};

struct messageStruct
{
    QString senderName;
    unsigned int senderID;
    QString text;
    QDateTime time;
};

struct advanUserInfo
{
    unsigned int id;
    QString name;
    QString login;
    QString desc;
    QString number;
};

struct advanChannelInfo
{
    unsigned int id;
    QString name;
    QString desc;
    bool isPublic;
    int countSubs;
    QString publicLogin;
};

struct advanGroupInfo
{
    unsigned int id;
    QString name;
    QString desc;
    int countUser;
    bool isPublic;
    int adminID;
    QString adminName;
    QString publicLogin;
};

#endif // DATA_STRUCT_H
