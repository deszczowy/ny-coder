/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "extensionbar.h"

#include "nyeditor.h"

ExtensionBar::ExtensionBar(NyEditor *parent) : QWidget(parent)
{
    connectedEditor = parent;
}

QSize ExtensionBar::sizeHint() const {
    return QSize(connectedEditor->LineNumberAreaWidth(), 0);
}


void ExtensionBar::paintEvent(QPaintEvent *event){
    connectedEditor->LineNumberAreaPaintEvent(event);
}

