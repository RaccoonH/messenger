#include "messagewidget.h"

MessageWidget::MessageWidget(Message *msg, QWidget *parent) : QWidget(parent)
{
    _msg = msg;

    _layout = new QHBoxLayout(this);
    _text = new QLabel(_msg->getText(), this);

    _layout->addWidget(_text);
}
