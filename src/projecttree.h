#ifndef PROJECTTREE_H
#define PROJECTTREE_H

#include "QTreeWidget"

class ProjectTree
{
public:
    ProjectTree(QTreeWidget *tree, QString path, QStringList filter);

private:
    QTreeWidget *_tree;
    QString _path;
    QString _name;
    QString _filter;

    void buildStructure();
    void processDirectory(QTreeWidgetItem *item, QString &path);

    QString rootName(const QString &path);
    QString buildFilter(QStringList &filters);
};

#endif // PROJECTTREE_H
