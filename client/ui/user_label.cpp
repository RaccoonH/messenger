#include "user_label.h"

UserLabel::UserLabel(unsigned int id, QString name, QWidget *parent) : QWidget(parent)
{
    _id = id;
    _name = name;
    layout = new QVBoxLayout(this);
    label = new QLabel(_name, this);
    layout->addWidget(label);

    setAutoFillBackground(true);
}

void UserLabel::fillLabel()
{
    _isFill = true;
    _pall.setColor(QPalette::Window,Qt::red);
    setPalette(_pall); 
}

void UserLabel::unfillLabel()
{
    _isFill = false;
    setPalette(this->style()->standardPalette());
}

bool UserLabel::isFill()
{
    return _isFill;
}

void UserLabel::mousePressEvent(QMouseEvent *event)
{

    emit clicked(_id);
}

UserLabel::~UserLabel()
{

}
