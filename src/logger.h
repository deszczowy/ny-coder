#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QTextEdit>


class Logger
{
public:
    static Logger& Instance();
    static void Write(QString message);
    static void Bind(QTextEdit *widget);

    void AddMessage(QString message);
    void BindWidget(QTextEdit *widget);
private:
    Logger();
    Logger(const Logger &);
    Logger& operator=(const Logger&);
    ~Logger();

private:
    QString _logPath;
    QTextEdit *_outputWidget;
};

#endif // LOGGER_H
