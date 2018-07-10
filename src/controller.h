/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QUuid>

#include "nyquist.h"

class Controller
{
public:
    Controller();

    // Nyquist interaction
    QProcess *GetNyquistProcess();
    void Start();
    void Restart();
    void Shutdown();
    void Break();
    void Replay();
    void SetupProject(QString projectPath);
    bool ExecuteFile(QString sourceFile);

private:
    Nyquist _nyquist;
};

#endif // CONTROLLER_H
