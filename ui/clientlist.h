#ifndef CLIENTLIST_H
#define CLIENTLIST_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QScrollArea>
#include <ui/clientlabel.h>
#include <ui/settingsui.h>

class ClientList : public QScrollArea
{
    Q_OBJECT
public:
    explicit ClientList(QWidget *parent = nullptr);
    ~ClientList();

public:
    void addNewClient(unsigned int id, UserInfo *userInfo, Chat *chat);

private:
    QPalette _pall;
    QVBoxLayout *_layout;
    QWidget *_clientListWidget;

signals:
    void addedNewClient(ClientLabel *clientLabel);
};

#endif // CLIENTLIST_H
