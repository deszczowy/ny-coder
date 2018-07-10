/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef FRAME_H
#define FRAME_H

class Frame
{
public:
    Frame();
    Frame(double time, double value);
    double Time();
    double Value();
private:
    double _time;
    double _value;
};

#endif

// FRAME_H
