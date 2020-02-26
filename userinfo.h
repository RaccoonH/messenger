#ifndef USERINFO_H
#define USERINFO_H

#include <QString>

class UserInfo
{
public:
    UserInfo(QString name, QString desc);
    QString getName();
    QString getDesc();
    void setName(QString name);
    void setDesc(QString desc);

private:
    QString _name;
    QString _desc;

};

#endif // USERINFO_H
