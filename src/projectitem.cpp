/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "projectitem.h"

#include <QFileInfo>

#include <src/storage/storage.h>
#include <src/storage/labels.h>

ProjectItem::ProjectItem(QTreeWidgetItem *parent, QStringList nodeName, QString filePath, QString relative, bool isDirectory)
    : QTreeWidgetItem(parent, nodeName), _filePath(""), _fileRelativePath(""), _fileName("")
{
    _directory = isDirectory;
    if (QFile::exists(filePath)){
        _fileName = QFileInfo(filePath).fileName();
        _filePath = filePath;
        _fileRelativePath = relative;
    }

    if (_directory){
        setIcon(0, Storage::getInstance().icon(Labels::ICON_FOLDER_EMPTY));
    } else {
        setIcon(0, Storage::getInstance().icon(Labels::ICON_DOCUMENT_LISP));
    }
}

QString ProjectItem::getFilePath()
{
    return _filePath;
}

QString ProjectItem::getFileName()
{
    return _fileName;
}

QString ProjectItem::getRelativePath()
{
    return _fileRelativePath;
}

bool ProjectItem::isDirectory()
{
    return _directory;
}
