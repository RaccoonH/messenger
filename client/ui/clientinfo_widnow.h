#ifndef CLIENTINFO_WIDNOW_H
#define CLIENTINFO_WIDNOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include "connector.h"
#include "clientinfo.h"
#include "userinfo.h"
#include "channelinfo.h"
#include "groupinfo.h"
#include "ui/user_label.h"

class ClientInfoWidnow : public QDialog
{
    Q_OBJECT

public:
    explicit ClientInfoWidnow(ClientInfo *clientInfo, QWidget *parent = 0);
    ~ClientInfoWidnow();

signals:
	void deletedClient(unsigned int, int);
	void addedNewClient(unsigned int id, int type);

private slots:
    void onUserLabelClicked(unsigned int id);
	void onUserInfoClicked();
	void onLeaveButtonClicked();
	void onSubButtonClicked();
	void onAddedFriendsClicked();
	void onRemoveFriendClicked();

private:
    unsigned int _currentID = 0;
	ClientInfo* _info = nullptr;
    QMap<unsigned int, UserLabel*> _map;

};

#endif // CLIENTINFO_WIDNOW_H
