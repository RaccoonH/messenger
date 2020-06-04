#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>

class ClientInfo
{
public:
    ClientInfo();

    virtual QString getName() = 0;
    virtual QString getDesc() = 0;
    virtual unsigned int getID() = 0;

    virtual void setName(QString name) = 0;
    virtual void setDesc(QString desc) = 0;
    virtual void setID(unsigned int id) = 0;

    virtual ~ClientInfo() = 0;

protected:
    QString _name;
    QString _desc;
    unsigned int _id;

};

#endif // CLIENTINFO_H
