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
