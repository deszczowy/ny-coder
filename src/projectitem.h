/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QTreeWidgetItem>

class ProjectItem : public QTreeWidgetItem
{
public:
    ProjectItem(QTreeWidgetItem *parent, QStringList nodeName, QString filePath, QString relative, bool isDirectory);

    QString getFilePath();
    QString getFileName();
    QString getRelativePath();

    bool isDirectory();
private:
    QString _filePath;
    QString _fileName;
    QString _fileRelativePath;
    bool _directory;
};

#endif // PROJECTITEM_H
