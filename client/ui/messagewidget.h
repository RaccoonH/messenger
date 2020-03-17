#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <message.h>

class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessageWidget(Message *msg, QWidget *parent = nullptr);

public:

private:
    QHBoxLayout *_layout;
    QLabel *_text;
    Message *_msg;

};

#endif // MESSAGEWIDGET_H
