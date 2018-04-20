#include "editor.h"
#include "extensionbar.h"

#include <QPainter>
#include <QTextBlock>
#include <QTextStream>

#include <src/syntaxlisp.h>

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

Editor::Editor(QWidget *parent) : QPlainTextEdit(parent)
{
    _extensionBar = new ExtensionBar(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(UpdateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(UpdateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(SelectCurrentLine()));

    UpdateLineNumberAreaWidth(0);
    SelectCurrentLine();

    SyntaxLisp *syntax = new SyntaxLisp(document());

    //setStyleSheet("padding: 5px; border: 0; background-color: #fff;");
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

void Editor::LoadFile(QString path)
{
    _path = path;

    QFile ffile(_path);
    if (ffile.open(QFile::ReadOnly | QIODevice::Text)) {
        QTextStream in(&ffile);
        document()->setPlainText(in.readAll());
    }
}

QString Editor::GetPath()
{
    return _path;
}

QString Editor::GetContent()
{
    return toPlainText();
}

void Editor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    _extensionBar->setGeometry(QRect(cr.left(), cr.top(), LineNumberAreaWidth(), cr.height()));
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
