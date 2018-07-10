/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef SWISS_H
#define SWISS_H

#include <QString>


// swiss army knife
namespace SwissArmyKnife
{
    QString FileContent(QString fileName);

    void ReplaceAll(QString &where, QString pattern, QString replace);

    double Min(double a, double b);

    double Max(double a, double b);
       int Max(int a, int b);
}

#endif
// SWISS_H
