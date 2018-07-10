/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef STORAGE_H
#define STORAGE_H

#include <QSqlDatabase>
#include <QStringList>
#include <QString>

#include "icon.h"
#include "storagefile.h"

class Storage
{
public:
    static Storage& getInstance();

    QStringList nyquistWordsRegEx();
    QStringList lispWordsRegEx();
    QStringList completionData();

    QString nyquistPath();
    QString themeValue(QString key);

    QIcon icon(QString resource);
    QIcon icon(QString resource, QString color);

    bool projectLoaded();
    void projectLoaded(bool isLoaded);
private:
    Storage();
    Storage(const Storage &);
    Storage& operator=(const Storage&);
    ~Storage();

    QStringList _nyquistRegEx;
    QStringList _lispRegEx;
    QStringList _completionData;
private:
    StorageFile *_setup;
    StorageFile *_theme;
    StorageFile *_defaultTheme;

    Icon _icon;
    bool _projectLoaded;

private:
    void gatherData();
    void gatherNyquistWords();
    void gatherLispWords();
    void gatherCompletionData();
};

#endif // STORAGE_H
