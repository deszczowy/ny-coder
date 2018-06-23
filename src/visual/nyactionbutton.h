#ifndef NYACTIONBUTTON_H
#define NYACTIONBUTTON_H

#include <QAction>
#include <QPushButton>

class NyActionButton : public QPushButton
{
    Q_OBJECT

public:
    explicit NyActionButton(QWidget *parent = nullptr);
    void setAction(QAction *action);

public slots:
    void updateButtonStatusFromAction();

private:
    QAction *_action = nullptr;

};

#endif // NYACTIONBUTTON_H
