/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef NYNODEDIALOG_H
#define NYNODEDIALOG_H

#include <QDialog>

namespace Ui {
class NyNodeDialog;
}

class NyNodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NyNodeDialog(QWidget *parent = 0);
    ~NyNodeDialog();

    bool Response();
    QString Value();

    void Set(QString value);

private slots:
    void on_accept_clicked();
    void on_discard_clicked();

private:
    Ui::NyNodeDialog *ui;

private:
    QStringList _filters;
    bool _response;
};

#endif // NYNODEDIALOG_H
