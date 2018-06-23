#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QTreeWidgetItem>

class ProjectItem : public QTreeWidgetItem
{
public:
    ProjectItem(QTreeWidgetItem *parent, QStringList nodeName, QString filePath, bool isDirectory);

    QString getFilePath();
    QString getFileName();
    bool isDirectory();
private:
    QString _filePath;
    QString _fileName;
    bool _directory;
};

#endif // PROJECTITEM_H
