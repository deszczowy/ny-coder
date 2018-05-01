#ifndef SOURCEFILE_H
#define SOURCEFILE_H

#include <QString>

class SourceFile
{
public:
    SourceFile(QString path);

    QString Path();
    QString Content();

    bool Save(QString content);
    bool SaveAs(QString newPath, QString content);

    bool IsNew();

private:
    QString _path;
    QString _content;
    bool _untitled;

    void LoadSourceFile(QString path);
};

#endif // SOURCEFILE_H
