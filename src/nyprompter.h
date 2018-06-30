#ifndef PROMPTER_H
#define PROMPTER_H

#include <QCompleter>
#include <QWidget>

namespace Ui {
class NyPrompter;
}

class NyPrompter : public QWidget
{
    Q_OBJECT

signals:
    void ApplyPrompt(QString prompted);

public:
    explicit NyPrompter(QWidget *parent = 0);
    ~NyPrompter();

    void Update(QString prefix, QRect geometry);
    bool ProcessKey(QKeyEvent *key);
    bool IsPrompting();

private:
    Ui::NyPrompter *ui;
    QCompleter *_completer;
    QString _prefix;

    void FitContent();

    QString TextToPrompt();
};

#endif // PROMPTER_H
