#include "nynodedialog.h"
#include "ui_nynodedialog.h"

NyNodeDialog::NyNodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NyNodeDialog)
{
    ui->setupUi(this);
}

NyNodeDialog::~NyNodeDialog()
{
    delete ui;
}

void NyNodeDialog::on_accept_clicked()
{

}

void NyNodeDialog::on_discard_clicked()
{

}
