#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1000, 700);
    _stackedWidget = new QStackedWidget(this);
    setCentralWidget(_stackedWidget);

    _messengerWindow = new MessengerWindow(_stackedWidget);
    _stackedWidget->addWidget(_messengerWindow);
}

MainWindow::~MainWindow()
{

}
