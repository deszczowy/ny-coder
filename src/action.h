/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef ACTION_H
#define ACTION_H

enum Action: unsigned char
{
    // project
    OpenFolder = 0,

    // workspace
    SaveCurrentFile,
    SaveCurrentFileAs,
    SaveAllFiles,
    CloseFile,

    // application
    Quit,
};

#endif // ACTION_H
