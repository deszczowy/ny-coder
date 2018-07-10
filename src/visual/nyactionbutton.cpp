/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "nyactionbutton.h"

NyActionButton::NyActionButton(QWidget *parent) :
    QPushButton(parent)
{}

void NyActionButton::setAction(QAction *action)
{
    if (_action && _action != action) {
        disconnect(_action, &QAction::changed, this, &NyActionButton::updateButtonStatusFromAction);
        disconnect(this, &NyActionButton::clicked, _action, &QAction::trigger);
    }
    _action = action;
    updateButtonStatusFromAction();
    connect(action, &QAction::changed, this, &NyActionButton::updateButtonStatusFromAction);
    connect(this, &NyActionButton::clicked, _action, &QAction::trigger);
}

void NyActionButton::updateButtonStatusFromAction()
{
    if (!_action)
        return;
    /*
    setText(_action->text());
    setStatusTip(_action->statusTip());
    setToolTip(_action->toolTip());
    setIcon(_action->icon());
    setEnabled(_action->isEnabled());
    setCheckable(_action->isCheckable());
    setChecked(_action->isChecked());
    */
}
