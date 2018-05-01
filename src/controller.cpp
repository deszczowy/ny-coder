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

void Controller::Shutdown()
{
    _nyquist.Quit();
}

bool Controller::ExecuteFile(QUuid sourceUuid)
{/*
    SourceFile *file = _sources.Find(sourceUuid);

    if (file)
    {
        if (file->IsNew())
        {
            file->SaveAs();
        }
        _nyquist.LoadFile(fileName);
    }*/
}
