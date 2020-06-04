#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include "message.h"
#include "connector.h"

class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessageWidget(Message *msg, QWidget *parent = nullptr);
	~MessageWidget();

	void updateNewText();
	Message* getMessage();

signals:
	void clicked(Message *msg, MessageWidget* pointer);

protected:
	void mousePressEvent(QMouseEvent *event);

private:
    QHBoxLayout *_layout;
    QLabel *_text;
    Message *_msg;
    QLabel *_nameSender;

};

#endif // MESSAGEWIDGET_H
