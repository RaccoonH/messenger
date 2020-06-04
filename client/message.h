#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QDateTime>

class Message
{
public:
    Message(unsigned int idSender, QString name, QString text, QDateTime time);

    QString getText();
    unsigned int getSenderID();
    QString getSenderName();
    QDateTime getTime();
	void setText(QString text);

private:
    unsigned int _idSender;
    QString _senderName;
    QString _text;
    QDateTime _time;
};

#endif // MESSAGE_H
