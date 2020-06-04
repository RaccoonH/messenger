#include "login_window.h"

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *loginHLayout = new QHBoxLayout(this);
    setLayout(loginHLayout);
    QWidget *loginVLayoutWidget = new QWidget(this);
    QVBoxLayout *loginVLayout = new QVBoxLayout(loginVLayoutWidget);

    QLabel *headline = new QLabel("Messenger v0.1" ,loginVLayoutWidget);
    QFont textFont;
    textFont.setPixelSize(30);
    headline->setFont(textFont);

    QLabel *login = new QLabel("Login", loginVLayoutWidget);
    _lineLogin = new QLineEdit(loginVLayoutWidget);

    QLabel *pass = new QLabel("Password", loginVLayoutWidget);
    _linePass = new QLineEdit(loginVLayoutWidget);
    _linePass->setEchoMode(QLineEdit::Password);

    _errorLabelLogin = new QLabel("",this);
    QPalette textColor;
    textColor.setColor(QPalette::WindowText, Qt::red);
    _errorLabelLogin->setPalette(textColor);

    QPushButton *loginButton = new QPushButton("Sign in", loginVLayoutWidget);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);

    QPushButton *regButton = new QPushButton("Create new account", loginVLayoutWidget);
    connect(regButton, &QPushButton::clicked, this, &LoginWindow::onRegistClicked);

    loginVLayout->addWidget(headline);
    loginVLayout->addWidget(login);
    loginVLayout->addWidget(_lineLogin);
    loginVLayout->addWidget(pass);
    loginVLayout->addWidget(_linePass);
    loginVLayout->addWidget(_errorLabelLogin);
    loginVLayout->addWidget(loginButton);
    loginVLayout->addWidget(regButton);
    loginVLayoutWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    loginVLayout->setAlignment(Qt::AlignCenter);

    loginHLayout->addWidget(loginVLayoutWidget);

    connect(Connector::getInstance(), &Connector::loggedIn, this, &LoginWindow::onLoggedIn);
    connect(Connector::getInstance(), &Connector::errLogin, this, &LoginWindow::onErrLogin);
}

void LoginWindow::onLoginClicked()
{
    QString login = _lineLogin->text();
    QString password = _linePass->text();
    Connector::getInstance()->loginUser(login, password);
	if (Connector::getInstance()->isLogined())
	{
		emit onLoggedIn();
	}
}

void LoginWindow::onLoggedIn()
{
    emit loggedIn();
}

void LoginWindow::onErrLogin()
{
    _errorLabelLogin->setText("Failed to log in");
}

void LoginWindow::onRegistClicked()
{
    emit registButtonClick();
}
