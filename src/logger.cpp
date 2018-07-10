/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "logger.h"

#include <fstream>

#include <QDateTime>
#include <QDebug>
#include <QFile>

Logger &Logger::Instance()
{
    static Logger instance;
    return instance;
}

void Logger::Write(QString message)
{
    Instance().AddMessage(message);
}

void Logger::Bind(QTextEdit *widget)
{
    Instance().BindWidget(widget);
}

void Logger::AddMessage(QString message)
{
    if (0 != message.trimmed().compare("")){

        // file name
        QString fn = "LOG";
        fn.append(QDateTime::currentDateTime().toString("yyyyMMdd"));
        fn.append(".txt");

        // full message
        QString m = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss (zzz)");
        m.append(" : ");
        m.append(message);

        // console
        qDebug() << m;

        // file
        QFile f(fn);
        if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream t(&f);
            t << m;
            t << "\n";
            f.close();
        }

        // widget
        if (_outputWidget){
            _outputWidget->append(m);
        }
    }
}

void Logger::BindWidget(QTextEdit *widget)
{
    _outputWidget = widget;
}

Logger::Logger()
{}

Logger::~Logger()
{}
