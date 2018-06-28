#include "nyprompter.h"
#include "ui_nyprompter.h"

#include <QPlainTextEdit>
#include <QStringListModel>

#include <src/storage/storage.h>

NyPrompter::NyPrompter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NyPrompter)
{
    ui->setupUi(this);


    _completer = new QCompleter(new QStringListModel(Storage::getInstance().completionData()), this);
    _completer->setCaseSensitivity(Qt::CaseInsensitive);

    setAttribute(Qt::WA_ShowWithoutActivating);

    setStyleSheet("QWidget{border:1px solid red;}");

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowStaysOnTopHint;
    flags |= Qt::FramelessWindowHint;
    setWindowFlags(flags);

    setAutoFillBackground(false);
}

NyPrompter::~NyPrompter()
{
    delete ui;
}

void NyPrompter::Update(QString prefix, QRect geometry)
{
    if (prefix.length() > 1)
    {
    _completer->setCompletionPrefix(prefix);
    ui->listWidget->clear();

    if (_completer->completionModel()->rowCount() == 0){
        hide();
        return;
    }

    for (int row = 0; row < _completer->completionModel()->rowCount(); row++)
         {
           QModelIndex idx = (_completer->completionModel()->index(row, 0));
           QString s = _completer->completionModel()->data(idx).toString();
           ui->listWidget->addItem(s);
         }


        setGeometry(geometry);
        FitContent();
        show();
    }

    else {
        this->hide();
    }

}

bool NyPrompter::ProcessKey(QKeyEvent *key)
{
    bool processed = true;

    if (key->key() == Qt::Key_Down){
        ui->listWidget->setFocus();
        ui->listWidget->selectAll();
    }
    else
    if (key->key() == Qt::Key_Up){
        ui->listWidget->setFocus();
        ui->listWidget->clearSelection();
    }
    else
    if (key->key() == Qt::Key_Escape){
        hide();
    }
    else{
        processed = false;
    }

    return processed;
}

bool NyPrompter::IsPrompting()
{
    return !isHidden();
}

void NyPrompter::FitContent()
{
    int c = _completer->completionModel()->rowCount();
    int w = ui->listWidget->sizeHintForColumn(0);
    int h = ui->listWidget->sizeHintForRow(0) * c;
    h += 5;

    QRect g = geometry();
    g.setHeight(h);
    g.setWidth(w);
    setGeometry(g);
}
