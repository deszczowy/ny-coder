#include "controller.h"

Controller::Controller()
{
}

QProcess *Controller::GetNyquistProcess()
{
    return _nyquist.GetProcess();
}

void Controller::Start()
{
    _nyquist.Run();
}

void Controller::Restart()
{
    _nyquist.Restart();
}

void Controller::Shutdown()
{
    _nyquist.Quit();
}

void Controller::Break()
{
    _nyquist.Break();
}

bool Controller::ExecuteFile(QString sourceFile)
{
    _nyquist.LoadFile(sourceFile);
}
