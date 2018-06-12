#include "nyquist.h"
#include "storage.h"

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
    Send("(play (seq (osc 50 0.5)(osc 43 0.5)))");
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
