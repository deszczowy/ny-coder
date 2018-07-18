/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "nyprojectitem.h"

#include <src/storage/storage.h>
#include <src/storage/labels.h>

#include <QFileDialog>
#include <QTextStream>

#include "fstream"

NyProjectItem::NyProjectItem(const QString absoluteFilePath, const QString projectRootPath, NyProjectItem *parent)
{
    _parent = parent;
    _editor = nullptr;

    _isValid = false;
    _isOpen = false;

    SetFile(absoluteFilePath, projectRootPath);

    if (_isDirectory){
        _icon = Storage::getInstance().icon(Labels::ICON_FOLDER_EMPTY);
    } else {
        _icon = Storage::getInstance().icon(Labels::ICON_DOCUMENT_LISP);
    }
}

NyProjectItem::~NyProjectItem()
{
    qDeleteAll(_children);
}

void NyProjectItem::AppendChild(NyProjectItem *child)
{
    _children.append(child);
}

NyProjectItem *NyProjectItem::Child(int row)
{
    return _children.value(row);
}

NyProjectItem *NyProjectItem::Parent()
{
    return _parent;
}

int NyProjectItem::ChildCount() const
{
    return _children.count();
}

int NyProjectItem::Row() const
{
    if (_parent)
        return _parent->_children.indexOf(const_cast<NyProjectItem*>(this));

    return 0;
}

bool NyProjectItem::IsValid()
{
    return _isValid;
}

bool NyProjectItem::IsOpen()
{
    return _isOpen;
}

bool NyProjectItem::IsInProject()
{
    return _isInProject;
}

bool NyProjectItem::IsDirectory()
{
    return _isDirectory;
}

QIcon NyProjectItem::Icon()
{
    return _icon;
}

bool NyProjectItem::HasEditor(NyEditor *editor)
{
    return editor == _editor;
}

void NyProjectItem::SetFile(QString path, QString root)
{
    _isInProject = path.startsWith(root);

    int offset = 0;
    if (_isInProject){
        offset = root.length() +1;
    }

    QFileInfo info(path);

    _absolutePath = path;
    _relativePath = path.mid(offset);
    _fileName = info.fileName();
    _isDirectory = info.isDir();

    _isValid = true;
}

void NyProjectItem::Open(NyEditor *editor)
{
    _isOpen = true;
    _editor = editor;
}

void NyProjectItem::Open(bool open, NyEditor *editor)
{
    _isOpen = open;
    _editor = editor;

    QString content = "";
    QFile file(_absolutePath);

    if (file.open(QFile::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        content = in.readAll();
    }

    _editor->SetContent(content);
    _editor->HasBeenSaved(_fileName);
}

void NyProjectItem::ConnectEditor(NyEditor *editor)
{
    _editor = editor;
    _isOpen = true;
}

void NyProjectItem::Close()
{
    _editor = nullptr;
    _isOpen = false;
}

void NyProjectItem::Save()
{
    if (SaveOnDisc()){
        _editor->HasBeenSaved(_fileName);
    }
}

QString NyProjectItem::ScriptPath()
{
    return _isInProject ? _relativePath : _absolutePath;
}

QString NyProjectItem::AbsolutePath()
{
    return _absolutePath;
}

QString NyProjectItem::RelativePath()
{
    return _relativePath;
}

QString NyProjectItem::FileName()
{
    return _fileName;
}

bool NyProjectItem::SaveOnDisc()
{
    QString content = _editor->Content();

    std::ofstream out(_absolutePath.toStdString());
    out << content.toStdString();
    out.close();

    return !out.bad();
}
