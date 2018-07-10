/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "storagefile.h"

#include <QFile>
#include <QTextStream>

#include <fstream>

StorageFile::StorageFile(QString fileName)
{
    if (fileName != "")
    {
        LoadFromFile(fileName);
    }
}

QString StorageFile::Value(QString key, QString defaultValue)
{
    if (_storage.contains(key))
    {
        return _storage.value(key);
    }
    return defaultValue;
}

void StorageFile::SaveToFile(QString fileName, QString header)
{
    QString content = "; \n; " + header + "\n; \n\n";

    QMapIterator<QString, QString> iterator(_storage);
    while (iterator.hasNext())
    {
        iterator.next();
        content += iterator.key() + ": " + iterator.value() + "\n";
    }

    std::ofstream out(fileName.toStdString());
    out << content.toStdString();
    out.close();
}

void StorageFile::Clear()
{
    _storage.clear();
}

void StorageFile::Add(QString key, QString value)
{
    _storage.insert(key, value);
}

void StorageFile::LoadFromFile(QString fileName)
{
    QString path = BuildPath(fileName);

    QFile file(path);
    Clear();

    if (file.open(QFile::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
           QString line = in.readLine().trimmed();

           if (line.length() > 0 && line.at(0) != ';')
           {
               int pos = line.indexOf(":");

               QString first = line.mid(0, pos).trimmed();
               QString second = line.mid(pos +1, line.length() - pos).trimmed();

               if (second != ""){
                   Add(first, second);
               }
           }
        }
        file.close();
    }

}

QString StorageFile::BuildPath(QString data)
{
    if (
        data.endsWith(".theme")
     || data.endsWith(".setup")
    )
        return data;

    QString path = ":/theme/res/themes/";
    path.append(data);
    path.append(".theme");

    return path;
}
