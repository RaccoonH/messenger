#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessageWidget(QWidget *parent = nullptr);

public:

private:
    QHBoxLayout *_layout;
    QLabel *_text;

};

#endif // MESSAGEWIDGET_H
