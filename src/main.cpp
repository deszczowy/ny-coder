#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QStyleHints>
#include <src/storage/storagefile.h>
#include <src/logger.h>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    /* not working on X11 :( */
    //QFontDatabase::addApplicationFont(QString(":/otf/res/fonts/inconsolata.otf"));
    //QFontDatabase::addApplicationFont(QString(":/ttf/res/fonts/quicksand.ttf"));

    /*
    StorageFile sf("");
    sf.Add("test1", "test");
    sf.Add("test32", "to jest tekst testowy");
    sf.Add("test3", "1");
    sf.SaveToFile("test2.settings", "Dane testowe");
    */

    Logger::Write("start");

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
