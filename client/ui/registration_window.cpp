#include "registration_window.h"

RegistrationWindow::RegistrationWindow(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *registerHLayout = new QHBoxLayout(this);
    setLayout(registerHLayout);
    QWidget *registerVLayoutWidget = new QWidget(this);
    QVBoxLayout *registerVLayout = new QVBoxLayout(registerVLayoutWidget);

    QLabel *headline = new QLabel("Messenger v0.1" ,registerVLayoutWidget);
    QFont textFont;
    textFont.setPixelSize(30);
    headline->setFont(textFont);

    QLabel *name = new QLabel("Name", registerVLayoutWidget);
    _lineName = new QLineEdit(registerVLayoutWidget);

    QLabel *login = new QLabel("Login", registerVLayoutWidget);
    _lineLogin = new QLineEdit(registerVLayoutWidget);

    QLabel *pass = new QLabel("Password", registerVLayoutWidget);
    _linePass = new QLineEdit(registerVLayoutWidget);
    _linePass->setEchoMode(QLineEdit::Password);

    _errorLabelRegister = new QLabel("",this);
    QPalette textColor;
    textColor.setColor(QPalette::WindowText, Qt::red);
    _errorLabelRegister->setPalette(textColor);

    QPushButton *signButton = new QPushButton("Sign up", registerVLayoutWidget);
    connect(signButton, &QPushButton::clicked, this, &RegistrationWindow::onRegistClicked);

    QPushButton *loginButton = new QPushButton("I already have an account");
    connect(loginButton, &QPushButton::clicked, this, &RegistrationWindow::onLoginClicked);

    registerVLayout->addWidget(headline);
    registerVLayout->addWidget(name);
    registerVLayout->addWidget(_lineName);
    registerVLayout->addWidget(login);
    registerVLayout->addWidget(_lineLogin);
    registerVLayout->addWidget(pass);
    registerVLayout->addWidget(_linePass);
    registerVLayout->addWidget(_errorLabelRegister);
    registerVLayout->addWidget(signButton);
    registerVLayout->addWidget(loginButton);
    registerVLayoutWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    registerVLayout->setAlignment(Qt::AlignCenter);

    registerHLayout->addWidget(registerVLayoutWidget);

    connect(Connector::getInstance(), &Connector::registered, this, &RegistrationWindow::onRegistered);
    connect(Connector::getInstance(), &Connector::errRegistLoginExist, this, &RegistrationWindow::onErrRegistLoginExist);
}


void RegistrationWindow::onRegistClicked()
{
    if(_lineName->text().length() < 3)
    {
        _errorLabelRegister->setText("Name is too short");
        return;
    }
    if(_linePass->text().length() < 5)
    {
        _errorLabelRegister->setText("Password is too short");
        return;
    }
    if(_lineLogin->text().length() < 5)
    {
        _errorLabelRegister->setText("Login is too short");
        return;
    }
    Connector::getInstance()->registUser(_lineLogin->text(), _linePass->text(), _lineName->text());
}

void RegistrationWindow::onRegistered()
{
    emit registered();
}

void RegistrationWindow::onErrRegistLoginExist()
{
    _errorLabelRegister->setText("This login is already exists");
}

void RegistrationWindow::onLoginClicked()
{
    emit loginButtonClick();
}
