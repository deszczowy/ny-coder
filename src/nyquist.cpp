/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "nyquist.h"
#include <src/storage/storage.h>
#include "swiss.h"

Nyquist::Nyquist()
{
    _isRunning = false;
    _process = new QProcess(this);
    _process->setProcessChannelMode(QProcess::MergedChannels);
}

Nyquist::~Nyquist()
{

}

/*
    Public Api
*/
void Nyquist::Run()
{
    if(_process){
        Start();
        PlayWelcomeTune();
    }
}

void Nyquist::Break()
{
    SendTop();
}

void Nyquist::Replay()
{
    Send("(r)");
}

void Nyquist::Quit()
{
    Stop();
}

void Nyquist::Restart()
{
    Start();
}

void Nyquist::LoadFile(QString fileName)
{
    QString command;
    command
        .append("(load \"")
        .append(fileName)
        .append("\")");
    Send(command);
}

void Nyquist::Command(QString command)
{
    Send(command);
}

QProcess *Nyquist::GetProcess()
{
    return _process;
}

/*
    Private nyquist methods
*/
void Nyquist::Start()
{
    if(_isRunning)
    {
        Stop();
    }

    if(_process)
    {
        _isRunning = true;
        QString p = Storage::getInstance().nyquistPath();
        _process->start(p, QIODevice::ReadWrite);
    }
}

void Nyquist::Stop()
{
    if(_isRunning)
    {
        Send("(break)");
        Send("(exit)");
        _process->close();
        _isRunning = false;
    }
}

void Nyquist::PlayWelcomeTune()
{
    QString tune = SwissArmyKnife::FileContent(":/script/res/scripts/welcome.lsp");
    Send(tune);
}

void Nyquist::SendTop()
{
    Send("(top)");
}

void Nyquist::Send(QString code)
{
    std::string command = code.toStdString().append("\n");
    if (_isRunning){
        _process->write(command.c_str()); //.toLatin1()
    }
}
