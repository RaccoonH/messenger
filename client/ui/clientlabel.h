#ifndef CLIENTLABEL_H
#define CLIENTLABEL_H

#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QStyle>
#include "user.h"
#include "channelinfo.h"
#include "userinfo.h"
#include "groupinfo.h"

class ClientLabel : public QWidget
{
    Q_OBJECT

public:
    explicit ClientLabel(unsigned int id, ClientInfo *clientInfo, QWidget *parent = nullptr);
    ~ClientLabel();

public:
    int getID();
    int getType();
    void fillLabel();
    void unfillLabel();
    void updateInfo();
	void showLogin();
	void setBackGroundColor(const QColor &col = nullptr);

private:
    QPalette _pall;
    ClientInfo *_clientInfo;
    unsigned int _idClient;
    QVBoxLayout *_layout;
    QLabel *_label;
    int _clientType;
    int _countNewMessages;
	QColor _col = Qt::gray;

protected:
    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked(unsigned int id, int clientType);
};

#endif // CLIENTLABEL_H
