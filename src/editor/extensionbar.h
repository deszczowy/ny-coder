/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef EXTENSIONBAR_H
#define EXTENSIONBAR_H

#include <QWidget>

class NyEditor;

class ExtensionBar : public QWidget
{
public:
    ExtensionBar(NyEditor *editor);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    NyEditor *connectedEditor;
};

#endif // EXTENSIONBAR_H
