#include "controller.h"
#include "swiss.h"
#include "storage/labels.h"

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

void Controller::Replay()
{
    _nyquist.Replay();
}

void Controller::SetupProject(QString projectPath)
{
    QString com = SwissArmyKnife::FileContent(Labels::SCRIPT_PROJECT_SETUP);
    SwissArmyKnife::ReplaceAll(com, "{path: project}", projectPath);
    _nyquist.Command(com); // pun intended
}

bool Controller::ExecuteFile(QString sourceFile)
{
    _nyquist.LoadFile(sourceFile);
    return true;
}
