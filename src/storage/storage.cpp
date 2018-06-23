#include "storage.h"
#include "labels.h"

#include <src/logger.h>

#include "QSqlQuery"
#include "QSqlRecord"
#include "QSqlError"
#include "QVector"

#include <QStandardPaths>
#include <QFile>
#include <QApplication>
#include <QDir>

Storage &Storage::getInstance()
{
    static Storage instance;
    return instance;
}

QStringList Storage::nyquistWordsRegEx()
{
    return _nyquistRegEx;
}

QStringList Storage::lispWordsRegEx()
{
    return _lispRegEx;
}

QStringList Storage::completionData()
{
    return _completionData;
}

QString Storage::nyquistPath()
{
    QString def = "ny";
    if (_setup){
        return _setup->Value("nyquist-path", def);
    } else {
        return def;
    }
}

QString Storage::themeValue(QString key)
{
    QString def = "#FFFFFF";
    if (_theme){
        return _theme->Value(
                    key,
                    _defaultTheme->Value(key, def)
               );
    } else {
        return def;
    }
}

QIcon Storage::icon(QString resource)
{
    return _icon.GetIcon(resource, themeValue("font"));
}

QIcon Storage::icon(QString resource, QString color)
{
    return _icon.GetIcon(resource, color);
}

Storage::Storage()
{
    _setup = new StorageFile("nyc.setup");
    _theme = new StorageFile(_setup->Value("theme", "default"));
    _defaultTheme = new StorageFile("default");

    QSqlDatabase m_db;

    if (QSqlDatabase::contains()){
        m_db = QSqlDatabase::database(QLatin1String(QSqlDatabase::defaultConnection), false);
    }else{
        m_db = QSqlDatabase::addDatabase("QSQLITE");
    }

#ifdef QT_DEBUG
    QString homeLocation = qApp->applicationDirPath();
#else
    QString homeLocation = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
#endif

    homeLocation += QDir::separator();
    homeLocation += "words.sqlite";

    QFile::copy(Labels::DATABASE_WORDS, homeLocation);

    m_db.setDatabaseName(homeLocation);

    if (!m_db.open())
    {
        Logger::Write("Error: connection with database fail");
    }
    else
    {
        Logger::Write("Database: connection ok");
        gatherData();
        m_db.close();
    }
}

Storage::~Storage()
{}

void Storage::gatherData()
{
    gatherLispWords();
    gatherNyquistWords();
    gatherCompletionData();
}

void Storage::gatherNyquistWords()
{
    _nyquistRegEx.clear();

    QString sql("SELECT regexp FROM nldata WHERE source = :name");
    QSqlQuery q;
    q.prepare(sql);
    q.bindValue(":name", "N");
    q.exec();

    if(q.record().count() > 0){
        int vLabel = q.record().indexOf("regexp");

        while (q.next()){
            _nyquistRegEx << q.value(vLabel).toString();
        }
    }
}

void Storage::gatherLispWords()
{
    _lispRegEx.clear();

    QString sql("SELECT regexp FROM nldata WHERE source = :name");
    QSqlQuery q;
    q.prepare(sql);
    q.bindValue(":name", "L");
    q.exec();

    if(q.record().count() > 0){
        int vLabel = q.record().indexOf("regexp");

        while (q.next()){
            _lispRegEx << q.value(vLabel).toString();
        }
    }
}

void Storage::gatherCompletionData()
{
    _completionData.clear();

    QString sql("SELECT call FROM nldata");
    QSqlQuery q;
    q.prepare(sql);
    q.exec();

    if(q.record().count() > 0){
        int vLabel = q.record().indexOf("call");

        while (q.next()){
            _completionData << q.value(vLabel).toString();
        }
    }
}
