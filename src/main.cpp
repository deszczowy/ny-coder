#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <src/storage/storagefile.h>

int main(int argc, char *argv[])
{

    /* not working on X11 :(
    QFontDatabase::addApplicationFont(":/otf/res/fonts/inconsolata.otf");
    QFontDatabase::addApplicationFont(":/ttf/res/fonts/quicksand.ttf");
    */

    StorageFile sf("");
    sf.Add("test1", "test");
    sf.Add("test32", "to jest tekst testowy");
    sf.Add("test3", "1");
    sf.SaveToFile("test2.settings", "Dane testowe");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
