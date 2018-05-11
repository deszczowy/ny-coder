#ifndef STYLER_H
#define STYLER_H

#include <src/storage/storagefile.h>

#include <QColor>



class Styler
{
public:
    Styler();

    QString CssMain();

    // Code highlight
    QColor Lisp();
    QColor Nyquist();
    QColor Parenthesis();
    QColor Text();
    QColor Comment();
    QColor Highlight();

    // UI

    // QWidget basic appearance
    QColor Background();
    QColor Border();
    QColor Font();

private:
    StorageFile *_theme;
};

#endif // STYLER_H
