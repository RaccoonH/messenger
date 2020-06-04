#include "messagewidget.h"

MessageWidget::MessageWidget(Message *msg, QWidget *parent) : QWidget(parent)
{
    _msg = msg;

    _layout = new QHBoxLayout(this);
    _text = new QLabel(_msg->getText(), this);

    //fix this part where finding only users name
    QString nameSender = msg->getSenderName();
    _nameSender = new QLabel(this);
    _nameSender->setText(nameSender + ": ");

    _layout->addWidget(_nameSender);
    _layout->addWidget(_text);

	setMouseTracking(true);
}

Message* MessageWidget::getMessage()
{
	return _msg;
}

void MessageWidget::updateNewText()
{
	_text->setText(_msg->getText());
}

void MessageWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton)
	{
		emit clicked(_msg, this);
	}
}

MessageWidget::~MessageWidget()
{

}
