/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

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
