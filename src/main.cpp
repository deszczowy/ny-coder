#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QFontDatabase::addApplicationFont(":/otf/inconsolata.otf");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
