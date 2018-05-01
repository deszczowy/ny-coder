#include "projecttree.h"
#include "projectitem.h"

#include <QDir>

ProjectTree::ProjectTree(QTreeWidget *tree, QString path, QStringList filter)
{
    if (tree != NULL){
        _tree = tree;
        _name = rootName(path);
        _path = QDir::cleanPath(path);
        _filter = buildFilter(filter);

        buildStructure();
    }
}

void ProjectTree::buildStructure()
{
    QTreeWidgetItem *root = new QTreeWidgetItem(_tree, QStringList() << _name);
    processDirectory(root, _path);
}

QString ProjectTree::rootName(const QString &path)
{
    return QDir(path).dirName();
}

QString ProjectTree::buildFilter(QStringList &filters)
{
    return QString(
        filters
            .replaceInStrings(".", "\\.")
            .join("|")
            .prepend(".*(")
            .append(")")
    );
}

void ProjectTree::processDirectory(QTreeWidgetItem *item, QString &path)
{
    QDir directory(path);

    QFileInfoList infos = directory.entryInfoList(
                QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst);
    foreach (QFileInfo info, infos) {
        QString p = info.absoluteFilePath();
        if (info.isDir()){
            ProjectItem *child = new ProjectItem(item, QStringList() << rootName(p), p);
            processDirectory(child, p);
        } else {
            if(info.fileName().contains(QRegExp(_filter)))
            new ProjectItem(item, QStringList() << info.fileName(), p);
        }
    }
}
