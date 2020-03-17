#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Server *serv = new Server(this);
    serv->startServer(QHostAddress::LocalHost, 1234);

}

MainWindow::~MainWindow()
{

}
