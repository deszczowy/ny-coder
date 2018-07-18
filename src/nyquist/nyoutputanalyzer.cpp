#include "nyoutputanalyzer.h"
#include <src/logger.h>

NyOutputAnalyzer::NyOutputAnalyzer()
    : _outputData("")
    , _pathToPlotPoints("")
    , _plottedSoundDuration(0.0)
{}

void NyOutputAnalyzer::Analyze(QString data)
{
    _outputData = data;
    CheckPlotPointsSaved();
    CheckPlottingTrigger();
}

QString NyOutputAnalyzer::PointsPath()
{
    return _pathToPlotPoints;
}

double NyOutputAnalyzer::SoundDuration()
{
    return _plottedSoundDuration;
}

void NyOutputAnalyzer::CheckPlotPointsSaved()
{
    QString pathPattern = "s-plot: writing (.*) \\.\\.\\.";
    QString durationPattern = "Sound duration is ((\\d*)\\.(\\d*)|(\\d*))s";

    QRegExp r(pathPattern);

    if (r.indexIn(_outputData) != -1){
        _pathToPlotPoints = r.cap(1);
        Logger::Write(_pathToPlotPoints);
    }

    r.setPattern(durationPattern);
    //double d = 0.0;
    if (r.indexIn(_outputData) != -1){
        _plottedSoundDuration = 0.0;
        _plottedSoundDuration += r.cap(1).toDouble();
        /*
        if (r.captureCount() > 2){
            d += QString("0." + r.cap(2)).toDouble();
        }
        */
    }
    //_plottedSoundDuration = d;
}

void NyOutputAnalyzer::CheckPlottingTrigger()
{
    if (_outputData.indexOf(" points from") >= 0) emit Plot();
}
