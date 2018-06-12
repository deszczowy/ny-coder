#ifndef STORAGEFILE_H
#define STORAGEFILE_H

#include <QMap>


class StorageFile
{
public:
    StorageFile(QString fileName);
    QString Value(QString key, QString defaultValue);

    void SaveToFile(QString fileName, QString header);

    void Clear();
    void Add(QString key, QString value);

private:
    QMap<QString, QString> _storage;

    void LoadFromFile(QString fileName);
    QString BuildPath(QString data);
};

#endif // STORAGEFILE_H
