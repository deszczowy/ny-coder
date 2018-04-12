#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include "QTreeWidgetItem"

class ProjectItem : public QTreeWidgetItem
{
public:
    ProjectItem(QTreeWidgetItem *parent, QStringList nodeName, QString filePath);

    QString getFilePath();
    QString getFileName();
private:
    QString filePath;
    QString fileName;
};

#endif // PROJECTITEM_H
