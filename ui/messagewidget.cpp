#include "messagewidget.h"

MessageWidget::MessageWidget(QWidget *parent) : QWidget(parent)
{
    _layout = new QHBoxLayout(this);
    _text = new QLabel("textMSG", this);

    _layout->addWidget(_text);
}
