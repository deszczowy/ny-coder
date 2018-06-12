#ifndef STORAGE_H
#define STORAGE_H

#include <QSqlDatabase>
#include <QStringList>
#include <QString>

#include "storage/storagefile.h"

class Storage
{
public:
    static Storage& getInstance();

    QStringList nyquistWordsRegEx();
    QStringList lispWordsRegEx();

    QString nyquistPath();
    QString themeValue(QString key);

private:
    Storage();
    Storage(const Storage &);
    Storage& operator=(const Storage&);
    ~Storage();

    QStringList _nyquistRegEx;
    QStringList _lispRegEx;
private:
    StorageFile *_setup;
    StorageFile *_theme;
    StorageFile *_defaultTheme;

private:
    void gatherData();
    void gatherNyquistWords();
    void gatherLispWords();
};

#endif // STORAGE_H
