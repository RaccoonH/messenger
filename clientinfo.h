#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>

class ClientInfo
{
public:
    ClientInfo();

    virtual QString getName() = 0;
    virtual QString getDesc() = 0;
    virtual void setName(QString name) = 0;
    virtual void setDesc(QString desc) = 0;

    virtual ~ClientInfo() = 0;

protected:
    QString _name;
    QString _desc;

};

#endif // CLIENTINFO_H
