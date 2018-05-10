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
    QFile file(fileName);
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

               Add(first, second);
           }
        }
        file.close();
    }

}
