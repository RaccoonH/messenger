#ifndef CHATDISPLAY_H
#define CHATDISPLAY_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include "ui/settingsui.h"
#include "ui/chatwidget.h"

class ChatDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit ChatDisplay(QWidget *parent = nullptr);
    ~ChatDisplay();

public:

private:
    QPalette _pall;
    QVBoxLayout *_layout;
    QWidget *_headLine;
    QWidget *_entryField;
    ChatWidget *_chatWidget;

private:
    QWidget *createHeadline(QWidget* parent);
    QWidget *createEntryField(QWidget* parent);
};

#endif // CHATDISPLAY_H
