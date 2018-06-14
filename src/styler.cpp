#include "styler.h"
#include "toolbox.h"
#include <src/storage/storage.h>

QString Styler::CssMain()
{
    QString css = Toolbox::FileContent(":/css/res/styles/main.css");

    Toolbox::ReplaceAll(css, "$background", Storage::getInstance().themeValue("background"));
    Toolbox::ReplaceAll(css, "$font", Storage::getInstance().themeValue("font"));
    Toolbox::ReplaceAll(css, "$border", Storage::getInstance().themeValue("border"));
    Toolbox::ReplaceAll(css, "$string", Storage::getInstance().themeValue("string"));

    return css;
}
