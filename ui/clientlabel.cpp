#include "clientlabel.h"

ClientLabel::ClientLabel(QWidget *parent) : QLabel(parent)
{
    setText("test");
    setMouseTracking(true);
}

void ClientLabel::mousePressEvent(QMouseEvent *event)
{
    _pall.setColor(QPalette::Window,Qt::red);
    setPalette(_pall);
    setAutoFillBackground(true);
}

ClientLabel::~ClientLabel()
{

}




