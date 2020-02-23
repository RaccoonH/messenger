#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QScrollArea>
#include <ui/messagewidget.h>

class ChatWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();

public:
    void addNewMessage();

private:
    QWidget *_widget;
    QVBoxLayout *_layout;
    QPalette _pall;

};

#endif // CHATWIDGET_H
