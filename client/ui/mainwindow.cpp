#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1000, 700);
    _stackedWidget = new QStackedWidget(this);
    setCentralWidget(_stackedWidget);

    Connector::getInstance();

    _loginWindow = new LoginWindow(this);
    _stackedWidget->addWidget(_loginWindow);

    connect(_loginWindow, &LoginWindow::loggedIn, this, &MainWindow::onLoggedIn);
    connect(_loginWindow, &LoginWindow::registButtonClick, this, &MainWindow::onRegisButtonClicked);

}

void MainWindow::onLoggedIn()
{
    _messengerWindow = new MessengerWindow(_stackedWidget);
    _stackedWidget->addWidget(_messengerWindow);
    _stackedWidget->setCurrentWidget(_messengerWindow);
}

void MainWindow::onRegistered()
{
    _stackedWidget->setCurrentWidget(_loginWindow);
}

void MainWindow::onRegisButtonClicked()
{
    _regWindow = new RegistrationWindow(this);
    _stackedWidget->addWidget(_regWindow);
    _stackedWidget->setCurrentWidget(_regWindow);
    connect(_regWindow, &RegistrationWindow::registered, this, &MainWindow::onRegistered);
    connect(_regWindow, &RegistrationWindow::loginButtonClick, this, &MainWindow::onRegistered);
}

void MainWindow::loadMsgsFromServer()
{

}

MainWindow::~MainWindow()
{
    Connector::deinit();
}
