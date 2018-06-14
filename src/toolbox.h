#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QString>



namespace Toolbox
{
    QString FileContent(QString fileName);

    void ReplaceAll(QString &where, QString pattern, QString replace);

    double Min(double a, double b);

    double Max(double a, double b);
}

#endif // TOOLBOX_H
