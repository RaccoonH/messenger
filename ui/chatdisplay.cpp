#include "chatdisplay.h"

ChatDisplay::ChatDisplay(QWidget *parent) : QWidget(parent)
{
    _layout = new QVBoxLayout(this);

    _headLine = createHeadline(this);
    _chatWidget = new ChatWidget(this);
    _entryField = createEntryField(this);

//    _pall.setColor(QPalette::Window,Qt::gray);
//    setPalette(_pall);
//    setAutoFillBackground(true);

    _layout->setMargin(0);

    _layout->addWidget(_headLine);
    _layout->addWidget(_chatWidget);
    _layout->addWidget(_entryField);
}

QWidget* ChatDisplay::createHeadline(QWidget* parent)
{
    QWidget *headline = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(headline);

    QLabel *name = new QLabel("name", headline);
    ///this button is test
    QPushButton *testbutton = new QPushButton("test", headline);

    layout->addWidget(name);
    layout->addWidget(testbutton);

    headline->setMaximumHeight(MAX_HEIGHT_HEADLINE);
    return headline;
}

QWidget* ChatDisplay::createEntryField(QWidget *parent)
{
    QWidget *entryField = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(entryField);
    QTextEdit *textEdit = new QTextEdit(entryField);
    QPushButton *sendButton = new QPushButton(entryField);

    layout->addWidget(textEdit);
    layout->addWidget(sendButton);

    entryField->setMaximumHeight(MAX_HEIGHT_ENTRYFIELD);
    return entryField;
}

ChatDisplay::~ChatDisplay()
{

}
