#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

class Message
{
public:
    Message(unsigned int idSender, QString text);

    QString getText();
    unsigned int getSenderID();
private:
    unsigned int _idSender;
    QString _text;
};

#endif // MESSAGE_H
