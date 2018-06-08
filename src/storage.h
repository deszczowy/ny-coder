#ifndef STORAGE_H
#define STORAGE_H

#include <QSqlDatabase>
#include <QStringList>
#include <QString>


class Storage
{
public:
    static Storage& getInstance();

    QStringList nyquistWordsRegEx();
    QStringList lispWordsRegEx();

private:
    Storage();
    Storage(const Storage &);
    Storage& operator=(const Storage&);
    ~Storage();

    QStringList _nyquistRegEx;
    QStringList _lispRegEx;

    void gatherData();
    void gatherNyquistWords();
    void gatherLispWords();
};

#endif // STORAGE_H
