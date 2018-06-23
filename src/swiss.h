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
}

#endif
// SWISS_H
