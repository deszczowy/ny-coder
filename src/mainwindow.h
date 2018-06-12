//pip install --upgrade cppclean
//cppclean /home/krystian/Repo/ny-coder/src

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QPushButton>

#include <QProcess>
#include <QTextEdit>
#include <QSplitter>
#include <QTreeWidgetItem>

#include "controller.h"
#include "projecttree.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void closeEvent(QCloseEvent *event);

/*
    The nyquist output management
*/
private slots:
    void onStdoutAvailable();
    void onFinished(int, QProcess::ExitStatus);

/*
    The project structure
*/
private:
    ProjectTree *_project;

    void OpenNewTab(QString fileName, QString path);

private slots:
    void onProjectElementSelection(QTreeWidgetItem *item, int column);

/*
    The main ui section
*/
private:
    Ui::MainWindow *ui;
    Controller _controller;
    bool _fullscreen;
    bool _maximized;
    
    QSplitter *mainSplitter;
    QSplitter *editorSplitter;

    void BaseUiSettings();
    void EditorsSettings();
    void OutputSettings();
    void ConnectSlots();
    void BindShortcuts();

    void SetupTheme();

    void SetButtonGlyph(QPushButton *button, QString glyphPath);

private slots:
    void onGo();
    void onBreak();
    void onMenu();
    void onClear();
    void onRefresh();
    void onTest();
    void onFullscreen();
    void onToggleOutput();
    void onToggleProjectTree();

/*
    The menu section,
    all objects and methods.
*/
private:
    QMenu* mainMenu;

    QMenu* projectMenu;
    QAction* miOpenFolder;
    QAction* miReloadProject;
    QAction* miSaveCurrentFile;
    QAction* miSaveCurrentFileAs;
    QAction* miSaveAllFiles;
    QAction* miCloseCurrentFile;
    QAction* miCloseProject;

    QMenu* viewMenu;
    QAction* miSwitchNyquistOutput;
    QAction* miSwitchProjectStructure;
    QAction* miFullscreen;

    QMenu* nyquistMenu;
    QAction* miRunCurrentFile;
    QAction* miReplay;
    QAction* miBreak;
    QAction* miRefreshNyquist;
    QAction* miClearOutput;

    QAction* miPreferences;
    QAction* miQuitApplication;

    QAction* miTest;

    void ShowContextMenu(const QPoint &pos);
    void BuildMenuActionsStructure();
    void BindMenuItemsWithSlots();
    void DestroyMenuItems();

private slots:
    void onOpenFolder();
    void onSaveCurrentFile();
    void onSaveAllFiles();
    void onQuitApplication();

/*
    Tabs section
*/
private:
private slots:
    void onCLoseTab(int index);
};

#endif // MAINWINDOW_H
