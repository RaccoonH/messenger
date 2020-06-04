#ifndef CLIENTSETTINGS_WINDOW_H
#define CLIENTSETTINGS_WINDOW_H

#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedWidget>
#include <QScrollArea>
#include "connector.h"
#include "clientinfo.h"
#include "user_label.h"
#include "ui/clientinfo_widnow.h"

class ClientSettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ClientSettingsWindow(ClientInfo *clientInfo, QWidget *parent = 0);
    ~ClientSettingsWindow();

public slots:
    void onExitButtonClicked();
    void onShowChannelUsersButtonClicked();
    void onShowGroupUsersButtonClicked();
    void onApplyButtonClicked();
    void onUserLabelClicked(unsigned int id);
    void onDeleteClicked();
    void onInfoClicked();
    void onInviteClicked();
	void onInviteChannelClicked();
    void onDeleteClientClicked();
    void onDeleteChannelClicked();
    void onDeleteGroupClicked();
	void onPublicButtonToggled();
    void backToSettings();

signals:
    void changedInfo(unsigned int id, int type);
    void deletedClient(unsigned int id, int type);

private:
    QStackedWidget *_stack;

    QWidget *clientSettingsWidget;
    QRadioButton *_isPublicButton;
    QLineEdit *_nameEdit;
    QLineEdit *_descEdit;
    QLabel *_errorLabel;
    QWidget *_listOfUsersWidget = nullptr;
    ClientInfo *_info = nullptr;
    QWidget *_checkDeleteWindow = nullptr;

	QLabel *_loginLabel;
	QLineEdit *_loginEdit;

    QWidget *_usersAreaWidget;
    QVBoxLayout *_usersLayout;

    QMap<unsigned int, UserLabel*> _map;
    unsigned int _currentID = 0;
    QPushButton *_deleteSub;
    QLineEdit *_login;
    QLabel *_errorLabelGroupUsers;

};

#endif // CLIENTSETTINGS_WINDOW_H
