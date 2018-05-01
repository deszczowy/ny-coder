#include "extensionbar.h"

#include "editor.h"

ExtensionBar::ExtensionBar(Editor *parent) : QWidget(parent)
{
    connectedEditor = parent;
}

QSize ExtensionBar::sizeHint() const {
    return QSize(connectedEditor->LineNumberAreaWidth(), 0);
}


void ExtensionBar::paintEvent(QPaintEvent *event){
    connectedEditor->LineNumberAreaPaintEvent(event);
}

