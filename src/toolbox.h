#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QString>



namespace Toolbox
{
    QString FileContent(QString fileName);

    void ReplaceAll(QString &where, QString pattern, QString replace);
}

#endif // TOOLBOX_H
