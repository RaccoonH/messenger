#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QCheckBox>
#include "connector.h"
#include "ui/clientlabel.h"
#include "ui/clientinfo_widnow.h"
#include "ui/clientlabel.h"
#include "ui/user_label.h"

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

public slots:
    void onFriendsButtonClicked();
	void onFriendListClicked(unsigned int id, int type);
	void onFriendsInfoClicked();

    void onAboutMeClicked();
    void onApplySettingsClicked();

    void onSearchSettingsClicked();
	void onSearchClicked();
	void onFoundClientLabelClicked(unsigned int id, int type);
	void onInfoClicked();
	void onAddedNewClient(unsigned int id, int type);

    void onNewChannelClicked();
    void onApplyNewChannelClicked();
	void onPublicChannelToggled();


    void onNewGroupClicked();
    void onApplyNewGroupClicked();
    void onFriendLabelClicked(unsigned int id);
	void onFriendLabelClickedChannel(unsigned int id);
	void onPublicGroupToggled();


    void backToSettings();

signals:
    void createdNewChannel(unsigned int id, ClientInfo* info);
	void addedNewClient(unsigned int id, ClientInfo* info);

private:
    QStackedWidget *_stackedWidget;

    QWidget *_settings = nullptr;

    QWidget *_friends = nullptr;
	QList<ClientLabel*> _listOfFriednLabels;
	unsigned int _currentFriendsID = 0;

    QWidget *_search = nullptr;
	QLineEdit *_searchLineEdit;
	QCheckBox *_usersSearch;
	QCheckBox *_channelsSearch;
	QCheckBox *_groupsSearch;
	QScrollArea *_clientArea;
	QWidget *_clientAreaWidget;
	QVBoxLayout *_clientLayout;
	unsigned int _currentFoundClientID = 0;
	int _currentFoundClientType = 0;
	QList<ClientLabel*> _listOfClientsLabel;

    QWidget *_newChannel = nullptr;
    QLineEdit *_nameEditNewChannel;
    QLineEdit *_descEditNewChannel;
    QRadioButton *_isPublicButtonNewChannel;
	QLabel *_publicLoginNewChannelLabel;
	QLineEdit *_publicLoginNewChannel;
	QLabel *_errorLabelChannel;
	QList<unsigned int> _listOfInvitedFriendsChannel;
	QMap<unsigned int, UserLabel*> _mapOfUsersLabelChannel;

    QWidget *_newGroup = nullptr;
    QLineEdit *_nameEditNewGroup;
	QLineEdit *_descEditNewGroup;
	QLabel *_publicLoginNewGroupLabel;
	QLineEdit *_publicLoginNewGroup;
	QLabel *_errorLabel;
    QRadioButton *_isPublicButtonNewGroup;
    QList<unsigned int> _listOfInvitedFriends;
    QMap<unsigned int, UserLabel*> _mapOfUsersLabel;

    QWidget *_aboutMe = nullptr;
	QLabel *_errorLabelAboutMe;


};

#endif // SETTINGS_WINDOW_H
