#include "projectitem.h"

#include <QFileInfo>

ProjectItem::ProjectItem(QTreeWidgetItem *parent, QStringList nodeName, QString filePath)
    : QTreeWidgetItem(parent, nodeName), fileName(""), filePath("")
{
    if (QFile::exists(filePath)){
        this->fileName = QFileInfo(filePath).fileName();
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
