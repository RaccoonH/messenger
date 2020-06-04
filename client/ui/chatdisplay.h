#ifndef CHATDISPLAY_H
#define CHATDISPLAY_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QMenu>
#include "ui/settingsui.h"
#include "ui/chatwidget.h"
#include "chat.h"
#include "connector.h"
#include "ui/clientinfo_widnow.h"
#include "ui/clientsettings_window.h"
#include <QDateTime>

class ChatDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit ChatDisplay(QWidget *parent = nullptr);
    ~ChatDisplay();

public:
    void addNewChat(Chat *chat);

public slots:
    void onClientLabelClicked(unsigned int id, int clientType);
    void onSendButtonClicked();
    void onClientInfoButtonClicked();

    //void onLoadedDialogChat(unsigned int id);
    //void onLoadedGroupChat(unsigned int id);
    //void onLoadedChannelChat(unsigned int id);

    void onLoadedNewDialogMessage(unsigned int id);
    void onLoadedNewGroupMessage(unsigned int id);
    void onLoadedNewChannelMessage(unsigned int id);

    void onSettingsClicked(unsigned int id);
    void onChangedInfo(unsigned int id, int type);
	void onDeletedClient(unsigned int id, int type);

	void onMessageWidgetClicked(Message *msg, MessageWidget *pointer);
	void onDeleteMessageClicked();
	void onEditMessageClicked();
	void onApplyEditMessageClicked();

	void onDeletedMessage(unsigned int id, QDateTime date);
	void onChangedMessage(Message*);

signals:
    void changedInfo(unsigned int id, int type);
    void deletedClient(unsigned int id, int type);

private:
    QPalette _pall;
    QVBoxLayout *_layout;
    QWidget *_headLine;
    QLabel *_userNameLabel;
    QPushButton *_sendButton;
    QWidget *_entryField;
    ChatWidget *_chatWidget;
    QTextEdit *_textEdit;
    QPushButton *_clientSettings;
	QMenu _menu;

    unsigned int _currentClientID = 0;
    int _currentClientType = 0;

	MessageWidget *_msgWidgetPointer;
	Message *_msg;
	QLineEdit *_lineEditMsg;

private:
    QWidget *createHeadline(QWidget* parent);
    QWidget *createEntryField(QWidget* parent);

};

#endif // CHATDISPLAY_H
