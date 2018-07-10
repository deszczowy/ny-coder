/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "projecttree.h"
#include "projectitem.h"
#include "logger.h"

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

void ProjectTree::Reload()
{
    reload();
}

void ProjectTree::buildStructure()
{
    QTreeWidgetItem *root = new QTreeWidgetItem(_tree, QStringList() << _name);
    processDirectory(root, _path);
    _tree->expandAll();
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
        int i = _path.length();
        QString r = p.mid(i+1);

        if (info.isDir()){
            ProjectItem *child = new ProjectItem(item, QStringList() << rootName(p), p, r, true);
            processDirectory(child, p);
        } else {
            if(info.fileName().contains(QRegExp(_filter))){
                new ProjectItem(item, QStringList() << info.fileName(), p, r, false);
            }
        }
    }
}

void ProjectTree::reload()
{
    Logger::Write("reload");
    _tree->clear();
    buildStructure();
}
