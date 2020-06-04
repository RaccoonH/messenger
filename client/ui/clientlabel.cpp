#include "clientlabel.h"
#include <QDebug>

ClientLabel::ClientLabel(unsigned int id, ClientInfo *clientInfo, QWidget *parent) : QWidget(parent)
{
    setAutoFillBackground(true);

    _layout = new QVBoxLayout(this);
    _label = new QLabel(this);
    _layout->addWidget(_label);

    _clientInfo = clientInfo;
    _idClient = id;

    if(typeid (*clientInfo) == typeid (UserInfo))
    {
        _clientType = 1;
        _label->setText("[us] " + _clientInfo->getName());
    }
    if(typeid (*clientInfo) == typeid (ChannelInfo))
    {
        _clientType = 2;
        _label->setText("[ch] " + _clientInfo->getName());
    }
    if(typeid (*clientInfo) == typeid (GroupInfo))
    {
        _clientType = 3;
        _label->setText("[gr] " + _clientInfo->getName());
    }

    setMouseTracking(true);
}

void ClientLabel::fillLabel()
{
    _pall.setColor(QPalette::Window, Qt::red);
    setPalette(_pall);
}

void ClientLabel::unfillLabel()
{
	if (_col == nullptr)
	{
		setPalette(this->style()->standardPalette());
	}
	else
	{
		_pall.setColor(QPalette::Window, _col);
		setPalette(_pall);
	}

}

void ClientLabel::showLogin()
{
	QLabel *login = new QLabel(this);
	switch (_clientType)
	{
	case 1:
	{
		login->setText("Login: " + dynamic_cast<UserInfo*>(_clientInfo)->getLogin());
		break;
	}

	case 2:
	{
		login->setText("Login: " + dynamic_cast<ChannelInfo*>(_clientInfo)->getPublicLogin());
		break;
	}

	case 3:
	{
		login->setText("Login: " + dynamic_cast<GroupInfo*>(_clientInfo)->getPublicLogin());
		break;
	}
	}
	_layout->addWidget(login);
}

int ClientLabel::getID()
{
    return _idClient;
}

int ClientLabel::getType()
{
    return _clientType;
}

void ClientLabel::updateInfo()
{
	switch (_clientType)
	{
	case 1:
	{
		_label->setText("[us] " + _clientInfo->getName());
		break;
	}
	case 2:
	{
		_label->setText("[ch] " + _clientInfo->getName());
		break;
	}
	case 3:
	{
		_label->setText("[gr] " + _clientInfo->getName());
		break;
	}
	}
}

void ClientLabel::setBackGroundColor(const QColor &col) 
{
	_col = col;
}

void ClientLabel::mousePressEvent(QMouseEvent *event)
{
    emit clicked(_idClient, _clientType);
}

ClientLabel::~ClientLabel()
{

}




