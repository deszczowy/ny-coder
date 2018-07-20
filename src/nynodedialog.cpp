#include "nynodedialog.h"
#include "ui_nynodedialog.h"

#include "styler.h"

NyNodeDialog::NyNodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NyNodeDialog)
{
    _response = false;
    _filters << ".lsp";
    ui->setupUi(this);

    Styler styler;
    setStyleSheet(styler.CssMain());

    ui->filterBox->setEditable(false);
    ui->filterBox->addItems(_filters);

    ui->label->setText("Enter file name");

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
    return ui->input->text() + ui->filterBox->currentText();
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
