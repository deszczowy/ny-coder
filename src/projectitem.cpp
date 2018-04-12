#include "projectitem.h"

#include "QFile"

ProjectItem::ProjectItem(QTreeWidgetItem *parent, QStringList nodeName, QString filePath)
    : QTreeWidgetItem(parent, nodeName), fileName(""), filePath("")
{
    if (QFile::exists(filePath)){
        this->fileName = QFile(filePath).fileName();
        this->filePath = filePath;
    }
}

QString ProjectItem::getFilePath()
{
    return this->filePath;
}

QString ProjectItem::getFileName()
{
    return this->fileName;
}
