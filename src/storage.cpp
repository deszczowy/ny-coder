#include "storage.h"

#include "QDebug"
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

Storage::Storage()
{
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
    qDebug() << homeLocation;

    QFile::copy(":db/res/db/words.sqlite", homeLocation);

    m_db.setDatabaseName(homeLocation);

    if (!m_db.open())
    {
      qDebug() << "Error: connection with database fail";
    }
    else
    {
      qDebug() << "Database: connection ok";

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
