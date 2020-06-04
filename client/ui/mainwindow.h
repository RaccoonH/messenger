#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "messenger_window.h"
#include "registration_window.h"
#include "login_window.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onLoggedIn();
    void onRegistered();
    void onRegisButtonClicked();

private:
    QStackedWidget *_stackedWidget;
    LoginWindow *_loginWindow;
    RegistrationWindow *_regWindow;
    MessengerWindow *_messengerWindow;

    void loadMsgsFromServer();
};

#endif // MAINWINDOW_H
