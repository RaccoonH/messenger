#include "message.h"

Message::Message(unsigned int idSender, QString text)
{
    _idSender = idSender;
    _text = text;
}

unsigned int Message::getSenderID()
{
    return _idSender;
}

QString Message::getText()
{
    return _text;
}
