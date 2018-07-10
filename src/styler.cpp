/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

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
