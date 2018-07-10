/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

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
