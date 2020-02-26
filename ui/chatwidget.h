#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QScrollArea>
#include <QList>
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

private:
    void clearChat();

private:
    QWidget *_widget;
    QVBoxLayout *_layout;
    QPalette _pall;
    Chat *_chat;

    QList<MessageWidget*> _listOfMsgWidget;

};

#endif // CHATWIDGET_H
