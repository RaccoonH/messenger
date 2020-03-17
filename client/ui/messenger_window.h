#ifndef MESSENGER_WINDOW_H
#define MESSENGER_WINDOW_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include "clientlist.h"
#include "chatdisplay.h"

class MessengerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MessengerWindow(QWidget *parent = 0);
        ~MessengerWindow();

private slots:
    void addNewChat(ClientLabel *label);

private:
    QHBoxLayout *_messengerWindowLayout;
    ClientList *_clientList;
    ChatDisplay *_chatDisplay;

private:
};

#endif // MESSENGER_WINDOW_H
