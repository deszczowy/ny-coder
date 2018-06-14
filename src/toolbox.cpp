#include "toolbox.h"

#include <QFile>
#include <QString>

namespace Toolbox{


QString FileContent(QString fileName)
{
    QString result = "";

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        result = file.readAll();
    }
    file.close();
    return result;
}

void ReplaceAll(QString &where, QString pattern, QString replace)
{
    where = where.replace(pattern, replace);
}

double Min(double a, double b)
{
    return a < b ? a : b;
}

double Max(double a, double b)
{
    return a > b ? a : b;
}

}
