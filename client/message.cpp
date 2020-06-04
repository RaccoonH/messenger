#include "message.h"

Message::Message(unsigned int idSender, QString name, QString text, QDateTime time)
{
    _idSender = idSender;
    _senderName = name;
    _text = text;
    _time = time;
}

unsigned int Message::getSenderID()
{
    return _idSender;
}

QString Message::getSenderName()
{
    return _senderName;
}

QString Message::getText()
{
    return _text;
}

QDateTime Message::getTime()
{
    return _time;
}

void Message::setText(QString text)
{
	_text = text;
}
