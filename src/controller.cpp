#include "controller.h"
#include "toolbox.h"

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

void Controller::SetupProject(QString projectPath)
{
    QString com = Toolbox::FileContent(":/script/res/scripts/projectsetup.lsp");
    Toolbox::ReplaceAll(com, "{path: project}", projectPath);
    //Toolbox::ReplaceAll(com, "\\", "\\\\");
    //
    _nyquist.Command(com); // pun intended
}

bool Controller::ExecuteFile(QString sourceFile)
{
    _nyquist.LoadFile(sourceFile);
}
