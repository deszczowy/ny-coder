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
