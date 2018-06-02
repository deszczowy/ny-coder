#include "styler.h"
#include "toolbox.h"

#include <QFile>

// defaults based on seed https://www.design-seeds.com/wander/wanderlust/a-door-hues-45/

Styler::Styler()
{
    _theme = new StorageFile("dark.theme");
}

QString Styler::CssMain()
{
    QString css = Toolbox::FileContent(":/css/res/styles/main.css");

    Toolbox::ReplaceAll(css, "$background", _theme->Value("background", "#2d384e"));
    Toolbox::ReplaceAll(css, "$font", _theme->Value("font", "#9fbac3"));
    Toolbox::ReplaceAll(css, "$border", _theme->Value("border", "#3b5787"));
    Toolbox::ReplaceAll(css, "$string", _theme->Value("string", "#ecece7"));

    return css;
}

QColor Styler::Lisp()
{
    return QColor(_theme->Value("lisp", "#d6dadf"));
}

QColor Styler::Nyquist()
{
    return QColor(_theme->Value("nyquist", "#d8ca8d"));
}

QColor Styler::Parenthesis()
{
    return QColor(_theme->Value("parenthesis", "#5b868f"));
}

QColor Styler::Text()
{
    return QColor(_theme->Value("string", "#ecece7"));
}

QColor Styler::Comment()
{
    return QColor(_theme->Value("comment", "#8dd8a6"));
}

QColor Styler::Highlight()
{
    return QColor(_theme->Value("selected-line", "#35435d"));
}

QColor Styler::Background()
{
    return QColor(_theme->Value("background", "#2d384e"));
}

QColor Styler::Border()
{
    return QColor(_theme->Value("border", "#3b5787"));
}

QColor Styler::Font()
{
    return QColor(_theme->Value("font", "#9fbac3"));
}
