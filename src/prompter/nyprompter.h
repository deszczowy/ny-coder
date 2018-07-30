/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

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
    explicit NyPrompter(QWidget *parent = nullptr);
    ~NyPrompter();

    void Update(QString prefix, QRect geometry);
    bool ProcessKey(QKeyEvent *key);
    bool IsPrompting();

private:
    Ui::NyPrompter *ui;
    QCompleter *_completer;
    QString _prefix;
    int _selectedIndex;

    void FitContent();
    void SelectNext();
    void SelectPrevious();
    void Deselect();

    QString TextToPrompt();
};

#endif // PROMPTER_H
