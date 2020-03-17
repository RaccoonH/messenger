#include "message.h"

Message::Message(QString text)
{
    _text = text;
}

QString Message::getText()
{
    return _text;
}
