#ifndef CLIENTLIST_H
#define CLIENTLIST_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <ui/clientlabel.h>
#include <ui/settingsui.h>
#include <ui/settings_window.h>

class ClientList : public QScrollArea
{
    Q_OBJECT
public:
    explicit ClientList(QWidget *parent = nullptr);
    ~ClientList();

public:
    void addNewClient(unsigned int id, ClientInfo *clientInfo);

public slots:
    void onChangedInfo(unsigned int id, int type);
    void onDeletedClient(unsigned int id, int type);

private slots:
    void onSettingsButtonClicked();
    void onClientLabelClicked(unsigned int id, int type);

private:
    QPalette _pall;
    QVBoxLayout *_layout;
    QWidget *_clientListWidget;
    QPushButton *_settingsButton;

    QList<ClientLabel*> _list;

signals:
    void addedNewClient(ClientLabel *clientLabel);
};

#endif // CLIENTLIST_H
