#ifndef CLIENTLABEL_H
#define CLIENTLABEL_H

#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <user.h>

class ClientLabel : public QWidget
{
    Q_OBJECT

public:
    explicit ClientLabel(unsigned int id, UserInfo *userInfo, Chat *chat, QWidget *parent = nullptr);
    ~ClientLabel();

public:


private:
    QPalette _pall;
    User *_user;
    QHBoxLayout *_layout;
    QLabel *_label;

protected:
    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked(Chat *chat);
};

#endif // CLIENTLABEL_H
