#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

class Message
{
public:
    Message(QString text);

    QString getText();
private:
    unsigned int idSender;
    QString _text;
};

#endif // MESSAGE_H
