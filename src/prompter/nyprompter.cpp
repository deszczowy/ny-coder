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

    setStyleSheet("QWidget{border:0px solid red;}");

    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowStaysOnTopHint;
    flags |= Qt::FramelessWindowHint;
    setWindowFlags(flags);

    setAutoFillBackground(false);

    _prefix = "";
    _selectedIndex = -1;
}

NyPrompter::~NyPrompter()
{
    delete ui;
}

void NyPrompter::Update(QString prefix, QRect geometry)
{
    if (_prefix != prefix)
    {
        _prefix = prefix;
        _selectedIndex = -1;

        _completer->setCompletionPrefix(_prefix);

        if (_completer->completionModel()->rowCount() == 1){
            QModelIndex idx = (_completer->completionModel()->index(0, 0));
            if (_completer->completionModel()->data(idx).toString().compare(_prefix) == 0){
                hide();
                return;
            }
        }

        if (_completer->completionModel()->rowCount() == 0){
            hide();
            return;
        }

        ui->listWidget->clear();

        int rc = _completer->completionModel()->rowCount();

        for (int row = 0; row < rc; row++)
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
        hide();
    }
}

bool NyPrompter::ProcessKey(QKeyEvent *key)
{
    bool processed = true;

    switch (key->key()){
    case Qt::Key_Down:
         SelectNext();
         break;
    case Qt::Key_Up:
         SelectPrevious();
         break;
    case Qt::Key_Escape:
         hide();
         break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
         emit ApplyPrompt(TextToPrompt());
         hide();
         break;
    default:
         processed = false;
         break;
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

    if (h > 200) h = 200;
    if (w > 200) w = 200;

    QRect g = geometry();
    g.setHeight(h);
    g.setWidth(w);
    setGeometry(g);
}

void NyPrompter::SelectNext()
{
    int c = ui->listWidget->count();

    if (c > 0){
        Deselect();
        _selectedIndex++;
        if (_selectedIndex >= c) _selectedIndex = 0;
        ui->listWidget->item(_selectedIndex)->setBackgroundColor(Storage::getInstance().themeValue("selected-line"));
        ui->listWidget->scrollToItem(ui->listWidget->item(_selectedIndex));
        ui->listWidget->repaint();
    } else {
        hide();
    }
}

void NyPrompter::SelectPrevious()
{
    int c = ui->listWidget->count();

    if (c > 0){
        Deselect();
        _selectedIndex--;
        if (_selectedIndex < 0) _selectedIndex = c -1;
        ui->listWidget->item(_selectedIndex)->setBackgroundColor(Storage::getInstance().themeValue("selected-line"));
        ui->listWidget->scrollToItem(ui->listWidget->item(_selectedIndex));
    } else {
        hide();
    }
}

void NyPrompter::Deselect()
{
    for (int i =0; i<ui->listWidget->count(); ++i){
        ui->listWidget->item(i)->setBackgroundColor(Storage::getInstance().themeValue("background"));
    }
}

QString NyPrompter::TextToPrompt()
{
    if (_selectedIndex >= 0){
        QString selected = ui->listWidget->item(_selectedIndex)->text();
        int part = selected.length() - _prefix.length();
        return selected.right(part);
    }

    return "";
}
