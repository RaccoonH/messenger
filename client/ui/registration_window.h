#ifndef REGISTRATION_WINDOW_H
#define REGISTRATION_WINDOW_H

#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include "connector.h"

class RegistrationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit RegistrationWindow(QWidget *parent = nullptr);

private:
    QLineEdit *_lineLogin;
    QLineEdit *_linePass;
    QLineEdit *_lineName;
    QLabel *_errorLabelRegister;

signals:
    void registered();
    void loginButtonClick();

public slots:
    void onRegistClicked();
    void onRegistered();
    void onErrRegistLoginExist();
    void onLoginClicked();
};

#endif // REGISTRATION_WINDOW_H
