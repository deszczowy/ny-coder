/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef NYPROJECTITEM_H
#define NYPROJECTITEM_H

#include <QIcon>
#include <QList>

#include <src/editor/nyeditor.h>

class NyProjectItem
{
public:
    explicit NyProjectItem(const QString absoluteFilePath, const QString projectRootPath, NyProjectItem *parent = 0);
    ~NyProjectItem();

public:
    void AppendChild(NyProjectItem *child);
    NyProjectItem *Child(int row);
    NyProjectItem *Parent();
    int ChildCount() const;
    int Row() const;
    bool HasEditor(NyEditor *editor);

public:
    void SetFile(QString path, QString root);
    void Open(NyEditor *editor);
    void Open(bool open, NyEditor *editor);
    void Rename(QString newName);
    void ConnectEditor(NyEditor *editor);
    void Close();
    void Save();

public:
    QString ScriptPath();
    QString AbsolutePath();
    QString RelativePath();
    QString FileName();
    bool IsValid();
    bool IsOpen();
    bool IsInProject();
    bool IsDirectory();
    QIcon Icon();

public:
    bool SaveOnDisc();

private:
    QList<NyProjectItem*> _children;
    NyProjectItem *_parent;
    NyEditor *_editor;

private:
    QString _absolutePath;
    QString _relativePath;
    QString _fileName;

private:
    bool _isValid;
    bool _isOpen;
    bool _isInProject;
    bool _isDirectory;

private:
    QIcon _icon;
};

#endif // NYPROJECTITEM_H
