#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "messenger_window.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QStackedWidget *_stackedWidget;
    MessengerWindow *_messengerWindow;
};

#endif // MAINWINDOW_H
