/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef NYQUIST_H
#define NYQUIST_H

#include <QProcess>

class Nyquist : public QObject
{

public:
    Nyquist();
    ~Nyquist();

    void Run();
    void Break();
    void Replay();
    void Quit();
    void Restart();
    void LoadFile(QString fileName);
    void Command(QString command);

    QProcess *GetProcess();
private:
    QProcess *_process;
    bool _isRunning;

    void Start();
    void Stop();
    void PlayWelcomeTune();
    void SendTop();
    void Send(QString code);
};

#endif

// NYQUIST_H
