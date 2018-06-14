#include "frame.h"

Frame::Frame() : _time(0), _value(0)
{}

Frame::Frame(double time, double value)
{
    _time = time;
    _value = value;
}

double Frame::Time()
{
    if (_time < 0) return 0;
    return _time;
}

double Frame::Value()
{
    if (_value > 1) return 1;
    if (_value < -1) return 0;
    return _value;
}
