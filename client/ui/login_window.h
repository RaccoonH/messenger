#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include "connector.h"

class LoginWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

private:
    QLineEdit *_lineLogin;
    QLineEdit *_linePass;
    QLabel *_errorLabelLogin;

signals:
    void loggedIn();
    void registButtonClick();

public slots:
    void onLoginClicked();
    void onRegistClicked();
    void onLoggedIn();
    void onErrLogin();
};

#endif // LOGIN_WINDOW_H
