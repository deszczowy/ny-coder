#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QUuid>

#include "nyquist.h"

class Controller
{
public:
    Controller();

    QProcess *GetNyquistProcess();

    void Start();
    void Shutdown();

    // actions on files
    bool NewFile(QUuid sourceUuid);
    bool SaveFile(QUuid sourceUuid);
    bool ExecuteFile(QUuid sourceUuid);
    bool CloseFile(QUuid sourceUuid);

private:
    Nyquist _nyquist;
};

#endif // CONTROLLER_H
