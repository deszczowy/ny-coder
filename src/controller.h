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
    void SetupProject(QString projectPath);
    bool ExecuteFile(QString sourceFile);

private:
    Nyquist _nyquist;
};

#endif // CONTROLLER_H
