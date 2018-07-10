/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QTextEdit>


class Logger
{
public:
    static Logger& Instance();
    static void Write(QString message);
    static void Bind(QTextEdit *widget);

    void AddMessage(QString message);
    void BindWidget(QTextEdit *widget);
private:
    Logger();
    Logger(const Logger &);
    Logger& operator=(const Logger&);
    ~Logger();

private:
    QString _logPath;
    QTextEdit *_outputWidget;
};

#endif // LOGGER_H
