/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef ICON_H
#define ICON_H

#include <QDomDocument>
#include <QIcon>

class NyIcon
{
public:
    NyIcon();

    QIcon GetIcon(QString resource, QString color);
private:
    void SetAttrRecur(QDomElement elem, QString strattr, QString strattrval);
};

#endif // ICON_H
