/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "nysoundframe.h"

NySoundFrame::NySoundFrame() : _time(0), _value(0)
{}

NySoundFrame::NySoundFrame(double time, double value)
{
    _time = time;
    _value = value;
}

double NySoundFrame::Time()
{
    if (_time < 0) return 0;
    return _time;
}

double NySoundFrame::Value()
{
    if (_value > 1) return 1;
    if (_value < -1) return 0;
    return _value;
}
