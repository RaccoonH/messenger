#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QScrollArea>
#include <QList>
#include <QMenu>
#include "ui/messagewidget.h"
#include "chat.h"

class ChatWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();

public:
    void addNewMessage(Message *msg);
	void selectChat(Chat *chat);
	void deleteMessageWidget(MessageWidget *pointer);
	void deleteMessageWidget(unsigned int id, QDateTime date);
	void editMessageWidget(MessageWidget *pointer);
	void editMessageWidget(Message*);
    Chat* getChat();

public slots:
	void onMessageWidgetClicked(Message *msg, MessageWidget *pointer);

signals:
	void messageWidgetClicked(Message *msg, MessageWidget *pointer);

private:
    void clearChat();

private:
	QMenu *_menu;
    QWidget *_widget;
    QVBoxLayout *_layout;
    QPalette _pall;
    Chat *_chat;

    QList<MessageWidget*> _listOfMsgWidget;

};

#endif // CHATWIDGET_H
