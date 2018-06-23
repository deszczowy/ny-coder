#include "styler.h"
#include "swiss.h"
#include <src/storage/storage.h>
#include <src/storage/labels.h>

QString Styler::CssMain()
{
    QString css = SwissArmyKnife::FileContent(Labels::CSS_MAIN);

    SwissArmyKnife::ReplaceAll(css, "$background", Storage::getInstance().themeValue("background"));
    SwissArmyKnife::ReplaceAll(css, "$font", Storage::getInstance().themeValue("font"));
    SwissArmyKnife::ReplaceAll(css, "$border", Storage::getInstance().themeValue("border"));
    SwissArmyKnife::ReplaceAll(css, "$string", Storage::getInstance().themeValue("string"));

    return css;
}
