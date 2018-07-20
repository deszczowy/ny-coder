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
