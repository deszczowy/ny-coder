#include <fstream>

#include <QFile>
#include <QTextStream>

#include "sourcefile.h"

SourceFile::SourceFile(QString path)
{
    _untitled = "" == path;
    _path = "";
    _content = "";

    if (!_untitled)
    {
        LoadSourceFile(path);
    }
}

QString SourceFile::Path()
{
    return _path;
}

QString SourceFile::Content()
{
    return _content;
}

bool SourceFile::IsNew()
{
    return _untitled;
}

void SourceFile::LoadSourceFile(QString path)
{
    QFile file(path);
    if (file.open(QFile::ReadOnly | QIODevice::Text))
    {
        _path = path;

        QTextStream in(&file);
        _content = in.readAll();
    }
}

bool SourceFile::Save(QString content)
{
    std::ofstream out(_path.toStdString());
    out << content.toStdString();
    out.close();

    return !out.bad();
}

bool SourceFile::SaveAs(QString newPath, QString content)
{
    _path = newPath;
    return Save(content);
}


