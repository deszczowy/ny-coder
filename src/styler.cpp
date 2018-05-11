#include "styler.h"
#include "toolbox.h"

#include <QFile>

Styler::Styler()
{
    _theme = new StorageFile("dark.theme");
}

QString Styler::CssMain()
{
    QString css = Toolbox::FileContent(":/css/res/styles/main.css");

    Toolbox::ReplaceAll(css, "$background", _theme->Value("background", "#f00"));
    Toolbox::ReplaceAll(css, "$font", _theme->Value("font", "#00f"));
    Toolbox::ReplaceAll(css, "$border", _theme->Value("border", "#0f0"));

    return css;
}

QColor Styler::Lisp()
{
    return QColor(_theme->Value("lisp", ""));
}

QColor Styler::Nyquist()
{
    return QColor(_theme->Value("nyquist", ""));
}

QColor Styler::Parenthesis()
{
    return QColor(_theme->Value("parenthesis", ""));
}

QColor Styler::Text()
{
    return QColor(_theme->Value("string", ""));
}

QColor Styler::Comment()
{
    return QColor(_theme->Value("comment", ""));
}

QColor Styler::Highlight()
{
    return QColor(_theme->Value("selected-line", ""));
}

QColor Styler::Background()
{
    return QColor(_theme->Value("background", ""));
}

QColor Styler::Border()
{
    return QColor(_theme->Value("border", ""));
}

QColor Styler::Font()
{
    return QColor(_theme->Value("font", ""));
}
