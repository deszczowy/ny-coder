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

private:
    void gatherData();
    void gatherNyquistWords();
    void gatherLispWords();
    void gatherCompletionData();
};

#endif // STORAGE_H
