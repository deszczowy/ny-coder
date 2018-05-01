#include "editor.h"
#include "extensionbar.h"

#include <QPainter>
#include <QTextBlock>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QTextStream>
#include <QStringListModel>
#include <QFile>
#include <QApplication>
#include <QFileDialog>

#include <src/syntaxlisp.h>
#include <src/sourcefile.h>

/*
QWidget
{
border: 0;
background-color: #fff;
}

QTextEdit#Editor
{
padding: 5px;
}
*/

Editor::Editor(QWidget *parent, QString filePath) : QPlainTextEdit(parent)
{
    _extensionBar = new ExtensionBar(this);

    // autocompletion
    completer = new QCompleter(this);
    completer->setModel(modelFromFile("/home/krystian/Repo/ny-coder/bin/model.txt"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    //completer->setWrapAround(false);

    SetCompleter(completer);
    //

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(UpdateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(UpdateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(SelectCurrentLine()));

    UpdateLineNumberAreaWidth(0);
    SelectCurrentLine();

    setFont(QFont("Inconsolata", 10, 1));


    SyntaxLisp *syntax = new SyntaxLisp(document());

    //setStyleSheet("padding: 5px; border: 0; background-color: #fff;");
    LoadFile(filePath);
}

Editor::~Editor(){
    delete _source;
}

void Editor::LineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(_extensionBar);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, _extensionBar->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int Editor::LineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 4 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void Editor::SetCompleter(QCompleter *completer)
{
    if (completer)
    {
        QObject::disconnect(completer, 0, this, 0);
    }

    _completer = completer;
    if(!_completer) return;

    _completer->setWidget(this);
    _completer->setCompletionMode(QCompleter::PopupCompletion);
    _completer->setCaseSensitivity(Qt::CaseInsensitive);

    QObject::connect(
        _completer,
        SIGNAL(activated(QString)),
        this,
        SLOT(InsertCompletion(QString))
                );
}

QCompleter *Editor::GetCompleter() const
{
    return _completer;
}

void Editor::LoadFile(QString path)
{
    _source = new SourceFile(path);
    document()->setPlainText(_source->Content());
}

QString Editor::Content()
{
    return toPlainText();
}

bool Editor::Save()
{
    if (!_source->IsNew())
    {
        if (_source->Save(Content()))
        {
            document()->setModified(false);
        }
    }
    else
    {
        SaveAs();
    }
}

bool Editor::SaveAs()
{
    QString name = QFileDialog::getSaveFileName(0, tr("Save file as..."), "~/", tr("Lisp source files (*.lsp)"));
    if (name != "")
    {
        if(_source->SaveAs(name, Content()))
        {
            document()->setModified(false);
        }
    }
}

void Editor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    _extensionBar->setGeometry(QRect(cr.left(), cr.top(), LineNumberAreaWidth(), cr.height()));
}

void Editor::keyPressEvent(QKeyEvent *event)
{
    if (_completer && _completer->popup()->isVisible()){
        switch (event->key()){
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            event->ignore();
            return;
        default:
            break;
        }
    }

    // shortcut = ctrl+e
    bool isShortCut = ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_E);

    if (!_completer || !isShortCut)
        QPlainTextEdit::keyPressEvent(event);

    const bool ctrlOrShift = event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if(!_completer || (ctrlOrShift && event->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");
    bool hasModifier = (event->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = TextUnderCursor();

    if (!isShortCut && (hasModifier || event->text().isEmpty() || completionPrefix.length() < 3 || eow.contains(event->text().right(1))))
    {
        _completer->popup()->hide();
        return;
    }

    if (completionPrefix != _completer->completionPrefix())
    {
        _completer->setCompletionPrefix(completionPrefix);
        _completer->popup()->setCurrentIndex(_completer->completionModel()->index(0, 0));
    }

    QRect rect = cursorRect();
    rect.setWidth(_completer->popup()->sizeHintForColumn(0)
                  + _completer->popup()->verticalScrollBar()->sizeHint().width()
                  );

    _completer->complete(rect);
}

void Editor::focusInEvent(QFocusEvent *event)
{
    if (_completer)
        _completer->setWidget(this);

    QPlainTextEdit::focusInEvent(event);
}

void Editor::UpdateLineNumberAreaWidth(int newBlockCount)
{
setViewportMargins(LineNumberAreaWidth(), 0, 0, 0);
}

void Editor::SelectCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::lightGray).lighter(125);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void Editor::UpdateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        _extensionBar->scroll(0, dy);
    else
        _extensionBar->update(0, rect.y(), _extensionBar->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        UpdateLineNumberAreaWidth(0);
}

void Editor::InsertCompletion(const QString &completion)
{
    if(_completer->widget() != this)
        return;

    QTextCursor cursor = textCursor();

    int extra = completion.length() - _completer->completionPrefix().length();

    cursor.movePosition(QTextCursor::Left);
    cursor.movePosition(QTextCursor::EndOfWord);
    cursor.insertText(completion.right(extra));

    setTextCursor(cursor);
}

QString Editor::TextUnderCursor() const
{
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    return cursor.selectedText();
}

QAbstractItemModel *Editor::modelFromFile(const QString & fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;
    while(!file.atEnd()){
        QByteArray line = file.readLine();
        if(!line.isEmpty())
            words << line.trimmed();
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer);
}