/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include <fstream>

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

#include <src/styler.h>
#include "syntaxlisp.h"

#include <src/storage/storage.h>
#include <src/logger.h>
#include <src/swiss.h>

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

Editor::Editor(QWidget *parent, QString filePath, QString relative) : QPlainTextEdit(parent)
{
    //_prompter = nullptr;
    _loaded = false;

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

    _relative = relative;

    LoadFile(filePath);
}

Editor::~Editor(){
}

void Editor::LineNumberAreaPaintEvent(QPaintEvent *event)
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

int Editor::LineNumberAreaWidth()
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

void Editor::LoadFile(QString path)
{
    QString content("");
    _path = "";
    _untitled = true;

    if (path != ""){
        QFile file(path);
        if (file.open(QFile::ReadOnly | QIODevice::Text))
        {
            _path = path;
            _fileName = QFileInfo(path).fileName();
            _untitled = false;

            QTextStream in(&file);
            content = in.readAll();
        }
    }
    document()->setPlainText(content);    
    _loaded = true;

    setWindowTitle(_fileName);
}

QColor Editor::Color(QString themeField)
{
    return QColor(Storage::getInstance().themeValue(themeField));
}

QRect Editor::PromptGeometry(QString word)
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

QString Editor::Content()
{
    return toPlainText();
}

QString Editor::Path()
{
    return _path;
}

QString Editor::FileName()
{
    return _fileName;
}

QString Editor::Relative()
{
    return _relative;
}

bool Editor::Save()
{
    if (!IsNew())
    {
        std::ofstream out(_path.toStdString());
        out << Content().toStdString();
        out.close();

        if (!out.bad())
        {
            document()->setModified(false);
            return true;
        }
        return true;
    }
    else
    {
        return SaveAs();
    }
}

bool Editor::SaveAs()
{
    QString name = QFileDialog::getSaveFileName(0, tr("Save file as..."), "~/", tr("Lisp source files (*.lsp)"));
    if (name != "")
    {
        _path = name;
        _untitled = false;

        if(Save())
        {
            document()->setModified(false);


            QString fn = QFileInfo(name).fileName();
            _fileName = fn;
            _pages->setTabText(_index, fn);

            return true;
        }
    }
    return false;
}

bool Editor::IsNew()
{
    return _untitled;
}

void Editor::SetContext(int index, QTabWidget *widget)
{
    _index = index;
    _pages = widget;
}

void Editor::SetPrompter(NyPrompter *prompter)
{
    _prompter = prompter;
    connect(_prompter, SIGNAL(ApplyPrompt(QString)), this, SLOT(onPrompt(QString)));
}

void Editor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    _extensionBar->setGeometry(QRect(cr.left(), cr.top(), LineNumberAreaWidth(), cr.height()));
}

void Editor::keyPressEvent(QKeyEvent *event)
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

        _prompter->Update(prefix, PromptGeometry(prefix));

    }
}

void Editor::keyReleaseEvent(QKeyEvent *event)
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

void Editor::focusInEvent(QFocusEvent *event)
{
    QPlainTextEdit::focusInEvent(event);
}

void Editor::UpdateLineNumberAreaWidth(int)
{
setViewportMargins(LineNumberAreaWidth(), 0, 0, 0);
}

void Editor::SelectCurrentLine()
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

void Editor::UpdateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        _extensionBar->scroll(0, dy);
    else
        _extensionBar->update(0, rect.y(), _extensionBar->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        UpdateLineNumberAreaWidth(0);
}


void Editor::SetModificationIndicator(bool modified)
{
    if (_loaded){
        QString caption = _pages->tabText(_index);

        if (modified){
            if (!caption.startsWith("* ")){
                caption.prepend("* ");
            }
        } else {
            if (caption.startsWith("* ")){
                caption = caption.mid(2);
            }
        }

        _pages->setTabText(_index, caption);
    }
}

void Editor::IncorporateLineIntoList()
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

void Editor::onPrompt(QString prompted)
{
    insertPlainText(prompted);
}

void Editor::CalculateIndent()
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

QString Editor::TextUnderCursor() const
{
    // all leters started with line begin, space or left parentheses
    QTextCursor c = textCursor();
    int pos = c.positionInBlock()-1;
    c.select(QTextCursor::BlockUnderCursor);
    QString s = c.selectedText().trimmed();
    int sp = s.lastIndexOf(' ', pos);
    int par = s.lastIndexOf('(', pos);

    sp = SwissArmyKnife::Max(sp, par);

    if (sp < 0) sp = 0; else sp++;
    return s.mid(sp, pos -sp +1).trimmed();
}
