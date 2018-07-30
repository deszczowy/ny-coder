/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef READER_H
#define READER_H

#include "nysoundframe.h"
#include <QString>
#include <QList>


class NyPlotData
{
public:
    NyPlotData(QString path);

    double Length();
    int Count();
    void Reset();
    bool HasNextFrame();
    bool HasFrames();
    NySoundFrame &Next();

private:
    QList<NySoundFrame> _frames;

    // properties
    double _max;
    double _min;
    double _length;

    int _cursor;

    void InitializeProperties();
    void Add(QStringList pair);
    void ProcessFile(QString path);
};

#endif // READER_H
