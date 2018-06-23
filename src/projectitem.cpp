#include "projectitem.h"

#include <QFileInfo>

#include <src/storage/storage.h>
#include <src/storage/labels.h>

ProjectItem::ProjectItem(QTreeWidgetItem *parent, QStringList nodeName, QString filePath, bool isDirectory)
    : QTreeWidgetItem(parent, nodeName), _filePath(""), _fileName("")
{
    _directory = isDirectory;
    if (QFile::exists(filePath)){
        _fileName = QFileInfo(filePath).fileName();
        _filePath = filePath;
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

bool ProjectItem::isDirectory()
{
    return _directory;
}
