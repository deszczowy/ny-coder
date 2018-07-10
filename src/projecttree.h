/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef PROJECTTREE_H
#define PROJECTTREE_H

#include <QTreeWidget>

class ProjectTree
{
public:
    ProjectTree(QTreeWidget *tree, QString path, QStringList filter);

    void Reload();
private:
    QTreeWidget *_tree;
    QString _path;
    QString _name;
    QString _filter;

    void buildStructure();
    void processDirectory(QTreeWidgetItem *item, QString &path);
    void reload();

    QString rootName(const QString &path);
    QString buildFilter(QStringList &filters);
};

#endif // PROJECTTREE_H
