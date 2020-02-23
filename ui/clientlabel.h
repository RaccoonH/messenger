#ifndef CLIENTLABEL_H
#define CLIENTLABEL_H

#include <QLabel>
#include <QWidget>

class ClientLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClientLabel(QWidget *parent = nullptr);
    ~ClientLabel();

private:
    QPalette _pall;

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // CLIENTLABEL_H
