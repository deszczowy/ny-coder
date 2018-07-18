#ifndef NYOUTPUTANALYZER_H
#define NYOUTPUTANALYZER_H

#include <QObject>

class NyOutputAnalyzer : public QObject
{
    Q_OBJECT
public:
    NyOutputAnalyzer();

public:
    void Analyze(QString data);

public:
    QString PointsPath();
    double SoundDuration();

signals:
    void Plot();

private:
    void CheckPlotPointsSaved();
    void CheckPlottingTrigger();

private:
    QString _outputData;
    QString _pathToPlotPoints;

    double _plottedSoundDuration;
};

#endif // NYOUTPUTANALYZER_H
