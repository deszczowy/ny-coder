/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "nyplotdata.h"
#include <src/swiss.h>

#include <QTextStream>

NyPlotData::NyPlotData(QString path)
{
    Reset();
    InitializeProperties();
    ProcessFile(path);
}

double NyPlotData::Length()
{
    return _length;
}

void NyPlotData::Reset()
{
    _cursor = 0;
}

NySoundFrame &NyPlotData::Next()
{
    _cursor++;
    return _frames[_cursor -1];
}

bool NyPlotData::HasNextFrame()
{
    return _cursor < _frames.count();
}

bool NyPlotData::HasFrames()
{
    return _frames.count() > 0;
}

int NyPlotData::Count()
{
    return _frames.count();
}

void NyPlotData::InitializeProperties()
{
    // this settings prevents properties stick to 0
    _min = 1.0;
    _max = -1.0;
    _length = 0;

    _frames.clear();
}

void NyPlotData::Add(QStringList pair)
{
    // contain key (time) and value (amplitude)
    if (2 == pair.count()){

        double one = pair[0].toDouble();
        double two = pair[1].toDouble();

        _max = SwissArmyKnife::Max(_max, two);
        _min = SwissArmyKnife::Min(_min, two);
        _length = SwissArmyKnife::Max(_length, one);

        _frames.append(NySoundFrame(one, two));
    }
}

void NyPlotData::ProcessFile(QString path)
{
    QFile file(path);
    if (file.open(QFile::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString line = "";
        QStringList parts;

        while (!in.atEnd())
        {
            line = in.readLine().trimmed();
            parts = line.split(" ", QString::SkipEmptyParts);
            Add(parts);
        }
        file.close();
    }
}



