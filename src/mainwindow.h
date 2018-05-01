//pip install --upgrade cppclean
//cppclean /home/krystian/Repo/ny-coder/src

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>

#include <QProcess>
#include <QTextEdit>
#include <QSplitter>
#include <QTreeWidgetItem>

#include "controller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private slots:

    void onStdoutAvailable();
    void onFinished(int, QProcess::ExitStatus);
    void on_goButton_clicked();
    void on_breakButton_clicked();



    void on_projectStructureView_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_clearOutput_clicked();

    void on_refreshOutput_clicked();
    void ShowContextMenu(const QPoint &pos);

    void on_menuButton_clicked();

private:
    Ui::MainWindow *ui;
    Controller _controller;
    
    QSplitter *mainSplitter;
    QSplitter *editorSplitter;

/*
    The menu section,
    all objects and methods.
*/
private:
    QMenu* mainMenu;

    QAction* miOpenFolder;
    QAction* miSaveCurrentFile;
    QAction* miSaveAllFiles;
    QAction* miQuitApplication;
    QAction* miRunCurrentFile;

    void BuildMenuActionsStructure();
    void BindMenuItemsWithSlots();
    void DestroyMenuItems();

private slots:
    void onOpenFolder();
    void onSaveCurrentFile();
    void onSaveAllFiles();
    void onQuitApplication();
    void onRunCurrentFile();

};

#endif // MAINWINDOW_H
