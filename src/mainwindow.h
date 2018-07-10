/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

//pip install --upgrade cppclean
//cppclean /home/krystian/Repo/ny-coder/src

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QPushButton>
#include <QHBoxLayout>

#include <QProcess>
#include <QTextEdit>
#include <QSplitter>
#include <QTreeWidgetItem>

#include <src/plotter/canvas.h>

#include <src/visual/nyactionbutton.h>

#include <src/editor/editor.h>

#include "controller.h"
#include "projecttree.h"

#include <src/prompter/nyprompter.h>

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

private:
    void BaseUiSettings();
    void BuildMenu();
    void BuildWorkspace();
    void CheckOutput(QString data);
    void ConnectSlots();
    void CreatePlotterCanvas();
    void CreateTransportButtons();
    void DestroyMenuNodes();
    void EditorsSettings();
    void OpenNewTab(QString fileName, QString path, QString relative);
    void OutputSettings();
    bool SaveAllFiles();
    void SetupTheme();
    void SetButtonGlyph(QPushButton *button, QString glyphPath);
    void ShowContextMenu(const QPoint &pos);


private slots:
    void onBreak();
    void onClear();
    void onCloseCurrentFile();
    void onCLoseTab(int index);
    void onFinished(int, QProcess::ExitStatus);
    void onFullscreen();
    void onGo();
    void onMenu();
    void onOpenFolder();
    void onProjectElementSelection(QTreeWidgetItem *item, int);
    void onRefresh();
    void onReloadProject();
    void onReplayLast();
    void onSaveAllFiles();
    void onSaveCurrentFile();
    void onSaveCurrentFileAs();
    void onStdoutAvailable();
    void onToggleOutput();
    void onTogglePlotter();
    void onToggleProjectTree();
    void onTest();
    void onQuit();
    void onQuitApplication();


private:
    void CreateActions();
    void BindShortcuts();
    void BindSlots();
    void BindActions();
    void DestroyActions();


private:
    // transporter
    QAction *_doMenu;
    QAction *_doRunScript;
    QAction *_doReplayLast;
    QAction *_doBreak;

    // project
    QAction *_doOpenProject;
    QAction *_doReloadProject;
    QAction *_doCloseProject;

    // files
    QAction *_doCloseCurrentFile;
    QAction *_doSaveCurrentFile;
    QAction *_doSaveCurrentFileAs;
    QAction *_doSaveAllFiles;

    // togglers
    QAction *_doToggleFullscreen;
    QAction *_doToggleOutputPanel;
    QAction *_doToggleProjectTree;
    QAction *_doTogglePlotter;

    // dialogs
    QAction *_doShowPreferences;
    QAction *_doShowHelp;

    // Nyquist
    QAction *_doNyquistRefresh;
    QAction *_doNyquistClearOutput;

    // Log
    QAction *_doLogClear;

    // Application
    QAction *_doQuit;

    // custom
    QAction *_doTest;


private:
    QMenu *_mainMenu;
    QMenu *_projectMenu;
    QMenu *_viewMenu;
    QMenu *_nyquistMenu;


private:
    Ui::MainWindow *ui;
    bool _fullscreen;
    bool _maximized;
    bool _allowQuit;


private:
    ProjectTree *_project;
    QSplitter *_mainSplitter;
    QSplitter *_workspaceSplitter;
    QSplitter *_editorSplitter;

    NyActionButton *_menuButton;
    NyActionButton *_runButton;
    NyActionButton *_replayButton;
    NyActionButton *_breakButton;
    QHBoxLayout *_transporterLayout;

private:
    Controller _controller;

private:
    QString _pointsPath;
    Canvas *_canvas;

private:
    NyPrompter *_promptr;
};

#endif // MAINWINDOW_H
