/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include <fstream>

#include "nyeditor.h"
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

#include <src/styler.h>
#include "syntaxlisp.h"

#include <src/storage/storage.h>
#include <src/logger.h>
#include <src/swiss.h>

NyEditor::NyEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    _extensionBar = new ExtensionBar(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(UpdateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(UpdateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(SelectCurrentLine()));
    connect(this, SIGNAL(modificationChanged(bool)), this, SLOT(SetModificationIndicator(bool)));

    UpdateLineNumberAreaWidth(0);
    SelectCurrentLine();

    #if defined(Q_OS_WIN)
    //setFont(QFont("Inconsolata", 12, 1));
    setFont(QFont("Fira Code Retina", 12, 1));
    #else
    setFont(QFont("Liberation Mono", 12, 1));
    #endif

    new SyntaxLisp(document());
}

NyEditor::~NyEditor(){
}

void NyEditor::LineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(_extensionBar);
    painter.fillRect(event->rect(), Color("background"));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Color("font"));
            painter.drawText(0, top, _extensionBar->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int NyEditor::LineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 16 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}


QColor NyEditor::Color(QString themeField)
{
    return QColor(Storage::getInstance().themeValue(themeField));
}

QRect NyEditor::PromptGeometry(QString word)
{
    /*
        editor position in relation to window + left gutter width + cursor position - word length in pixels
    */
    QRect t = geometry();
    QRect e = _extensionBar->geometry();

    QPoint w = mapTo(window(), t.topLeft());

    QRect r = cursorRect();
    QRect b = fontMetrics().boundingRect(word);
    QRect g(w.x() + r.x() - b.width() + e.width(),
            w.y() + r.y() + r.height() +2,
            100, 200);
    return g;
}

QString NyEditor::Content()
{
    return toPlainText();
}

void NyEditor::SetContent(QString content)
{
    setPlainText(content);
}

void NyEditor::HasBeenSaved(QString name)
{
    _pages->setTabText(_index, name);
    document()->setModified(false);
}

void NyEditor::SetContext(int index, QTabWidget *widget)
{
    _index = index;
    _pages = widget;
}

void NyEditor::SetPrompter(NyPrompter *prompter)
{
    _prompter = prompter;
    connect(_prompter, SIGNAL(ApplyPrompt(QString)), this, SLOT(onPrompt(QString)));
}

void NyEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    _extensionBar->setGeometry(QRect(cr.left(), cr.top(), LineNumberAreaWidth(), cr.height()));
}

void NyEditor::keyPressEvent(QKeyEvent *event)
{
    if (_prompter && _prompter->IsPrompting()){
        if(_prompter->ProcessKey(event)) return;
    }

    bool stop = false;
    switch (event->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        CalculateIndent();
        break;
    case Qt::Key_Tab:
        insertPlainText("    ");
        stop = true;
        break;
    default:
        break;
    }

    if (!stop){

        QPlainTextEdit::keyPressEvent(event);

        QString prefix = TextUnderCursor();
        Logger::Write(prefix);

        _prompter->Update(prefix, PromptGeometry(prefix));

    }
}

void NyEditor::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        insertPlainText(_indent);
        break;
    case Qt::Key_ParenLeft:
        insertPlainText(")");
        moveCursor(QTextCursor::Left, QTextCursor::MoveAnchor);
        break;
    default:
        QPlainTextEdit::keyReleaseEvent(event);
        break;
    }

    bool isMakeList = ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_L);

    if (isMakeList){
        IncorporateLineIntoList();
    }
}

void NyEditor::focusInEvent(QFocusEvent *event)
{
    QPlainTextEdit::focusInEvent(event);
}

void NyEditor::UpdateLineNumberAreaWidth(int)
{
setViewportMargins(LineNumberAreaWidth(), 0, 0, 0);
}

void NyEditor::SelectCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = Color("selected-line");

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void NyEditor::UpdateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        _extensionBar->scroll(0, dy);
    else
        _extensionBar->update(0, rect.y(), _extensionBar->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        UpdateLineNumberAreaWidth(0);
}


void NyEditor::SetModificationIndicator(bool modified)
{

    QString caption = _pages->tabText(_index);

    if (modified){
        if (!caption.startsWith("* ")){
            caption.prepend("* ");
        }
    }

    _pages->setTabText(_index, caption);

}

void NyEditor::IncorporateLineIntoList()
{
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::LineUnderCursor);
    QString c = "(";
    c.append(cursor.selectedText());
    c.append(")");
    cursor.removeSelectedText();
    setTextCursor(cursor);
    insertPlainText(c);
    moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
}

void NyEditor::onPrompt(QString prompted)
{
    insertPlainText(prompted);
}

void NyEditor::CalculateIndent()
{
    _indent = "";
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::LineUnderCursor);
    QString c = cursor.selectedText();

    int i = 0;
    if (c.length() > 0) {
        while (c.at(i) == ' ') i++;
        _indent = c.mid(0, i);
    }
}

QString NyEditor::TextUnderCursor() const
{
    // all leters started with line begin, space or left parentheses
    QTextCursor c = textCursor();
    int pos = c.positionInBlock()-1;
    c.select(QTextCursor::BlockUnderCursor);
    QString s = c.selectedText().trimmed();
    int sp = s.lastIndexOf(' ', pos);
    int par = s.lastIndexOf('(', pos);
    int nl = s.lastIndexOf('\n', pos);
    int cr = s.lastIndexOf('\r', pos);

    sp = SwissArmyKnife::Max(sp, par);
    nl = SwissArmyKnife::Max(nl, cr);
    sp = SwissArmyKnife::Max(sp, nl);

    if (sp < 0) sp = 0; else sp++;
    return s.mid(sp, pos -sp +1).trimmed();
}
