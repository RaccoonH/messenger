#ifndef USER_LABEL_H
#define USER_LABEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStyle>

class UserLabel : public QWidget
{
    Q_OBJECT
public:
    explicit UserLabel(unsigned int id, QString name, QWidget *parent = nullptr);
    void fillLabel();
    void unfillLabel();
    bool isFill();
    ~UserLabel();

private:
    unsigned int _id;
    QString _name;
    QVBoxLayout *layout;
    QLabel *label;
    QPalette _pall;
    bool _isFill = false;


protected:
    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked(unsigned int id);

public slots:
};

#endif // USER_LABEL_H
