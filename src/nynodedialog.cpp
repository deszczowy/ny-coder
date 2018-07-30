/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "nynodedialog.h"
#include "ui_nynodedialog.h"

#include "styler.h"

NyNodeDialog::NyNodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NyNodeDialog)
{
    _response = false;
    _filters << ".lsp";
    _filters << "directory";
    ui->setupUi(this);

    Styler styler;
    setStyleSheet(styler.CssMain());

    ui->filterBox->setEditable(false);
    ui->filterBox->addItems(_filters);

    ui->label->setText("Enter name");

    setWindowModality(Qt::ApplicationModal);
}

NyNodeDialog::~NyNodeDialog()
{
    delete ui;
}

bool NyNodeDialog::Response()
{
    return _response;
}

QString NyNodeDialog::Value()
{
    QString filter = ui->filterBox->currentText();
    QString value = ui->input->text();
    if (filter.compare("directory") != 0){
        value += ui->filterBox->currentText();
    }

    return value;
}

void NyNodeDialog::Set(QString value)
{
    int offset = value.lastIndexOf(".");

    if (offset > 0){
        value = value.mid(0, offset);
    }
    ui->input->setText(value);
}

void NyNodeDialog::on_accept_clicked()
{
    _response = true;
    hide();
}

void NyNodeDialog::on_discard_clicked()
{
    _response = false;
    hide();
}
