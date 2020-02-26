#include "clientlabel.h"
#include <QDebug>

ClientLabel::ClientLabel(unsigned int id, UserInfo *userInfo, Chat *chat, QWidget *parent) : QWidget(parent)
{
    _layout = new QHBoxLayout(this);
    _label = new QLabel(this);
    _layout->addWidget(_label);

    _user = new User(id, userInfo, chat);
    _pall.setColor(QPalette::Window, Qt::blue);
    _label->setText(_user->getUserInfo()->getName());


    setMouseTracking(true);
}

void ClientLabel::mousePressEvent(QMouseEvent *event)
{
    _pall.setColor(QPalette::Window,Qt::red);
    setPalette(_pall);
    setAutoFillBackground(true);
    emit clicked(_user->getChat());
}

ClientLabel::~ClientLabel()
{
    delete _user;
}




