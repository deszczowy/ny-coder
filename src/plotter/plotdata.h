#ifndef READER_H
#define READER_H

#include "frame.h"
#include <QString>
#include <QList>


class PlotData
{
public:
    PlotData(QString path);

    double Length();
    int Count();
    void Reset();
    bool HasNextFrame();
    bool HasFrames();
    Frame &Next();

private:
    QList<Frame> _frames;

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
