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

#endif // NYQUIST_H
