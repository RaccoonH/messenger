#include "chatwidget.h"

ChatWidget::ChatWidget(QWidget *parent) : QScrollArea(parent)
{
    _widget = new QWidget(this);
    _layout = new QVBoxLayout(_widget);

    _pall.setColor(QPalette::Window,Qt::white);
    setPalette(_pall);
    setAutoFillBackground(true);

    for(int i = 0; i < 15; i++)
    {
        addNewMessage();
    }
    setWidget(_widget);
}

void ChatWidget::addNewMessage()
{
    ///test messages, need class msgWidget
    MessageWidget *message = new MessageWidget(this);
    _layout->addWidget(message);
}

ChatWidget::~ChatWidget()
{

}
