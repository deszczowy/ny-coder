/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "mainwindow.h"
#include <ui_mainwindow.h>

#include "nynodedialog.h"
#include "preferences.h"

#include <src/storage/storage.h>
#include <src/styler.h>
#include <src/logger.h>

#include <fstream>
#include <iostream>

#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QShortcut>

#include <QCloseEvent>


#include "editor/nyeditor.h"

#include <src/storage/storagefile.h>
#include <src/storage/labels.h>

//
//
// Lifetime
//
//

/*
    Preferences *pref = new Preferences();
    pref->exec();
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _promptr = new NyPrompter(this);
    _analyzer = new NyOutputAnalyzer();

    CreateActions();

    BaseUiSettings();
    BuildMenu();
    BuildProjectMenu();
    SetupTheme();
    EditorsSettings();
    OutputSettings();
    ConnectSlots();

    _controller.Start();
}

MainWindow::~MainWindow()
{
    DestroyActions();
    DestroyMenuNodes();
    delete ui;
}

//
//
// UI Settings
//
//

void MainWindow::BaseUiSettings()
{
    ui->setupUi(this);
    setWindowTitle("Nyquist Coder :: version 0.0");

    Logger::Bind(ui->outputLogArea);

    _fullscreen = false;
    _maximized = true;

    BuildWorkspace();
    showMaximized();
}

void MainWindow::EditorsSettings()
{
    ui->editorMain->setTabsClosable(true);
    ui->editorAdditional->setTabsClosable(true);
    ui->editorAdditional->hide();

    ui->projectStructureView->header()->hide();
}

void MainWindow::OutputSettings()
{
    ui->outputNyquistArea->setFont(QFont("Courier", 12, 1));
}

void MainWindow::ConnectSlots()
{
    // nyquist output
    connect(_controller.GetNyquistProcess(), SIGNAL(readyReadStandardOutput()), this, SLOT(onStdoutAvailable()) );
    connect(_controller.GetNyquistProcess(), SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)) );

    // output buttons
    connect(ui->clearOutput, SIGNAL(clicked(bool)), this, SLOT(onClear()) );
    connect(ui->refreshOutput, SIGNAL(clicked(bool)), this, SLOT(onRefresh()) );

    // editors
    connect(ui->editorMain, SIGNAL(tabCloseRequested(int)), this, SLOT(onCLoseTab(int)));

    // project
    connect(ui->projectStructureView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onProjectElementSelection(QModelIndex)) );

    // output
    connect(_analyzer, SIGNAL(Plot()), this, SLOT(onPlot()) );
}

void MainWindow::CreatePlotterCanvas()
{
    _canvas = new Canvas(this);
    _canvas->setMinimumSize(200, 100);
    _canvas->SetPen(QColor(Storage::getInstance().themeValue("font")));
    _canvas->SetBrush(QColor(Storage::getInstance().themeValue("background")));
    _canvas->setVisible(false);
}

void MainWindow::CreateTransportButtons()
{
    if (ui->transportPane){
        _transporterLayout = new QHBoxLayout(ui->transportPane);

        _menuButton = new NyActionButton(ui->transportPane);
        _runButton = new NyActionButton(ui->transportPane);
        _replayButton = new NyActionButton(ui->transportPane);
        _breakButton = new NyActionButton(ui->transportPane);

        _menuButton->setAction(_doMenu);
        _runButton->setAction(_doRunScript);
        _replayButton->setAction(_doReplayLast);
        _breakButton->setAction(_doBreak);

        SetButtonGlyph(_menuButton, Labels::ICON_MENU);
        SetButtonGlyph(_runButton, Labels::ICON_TRANSPORTER_PLAY);
        SetButtonGlyph(_replayButton, Labels::ICON_TRANSPORTER_REPLAY);
        SetButtonGlyph(_breakButton, Labels::ICON_TRANSPORTER_STOP);

        _transporterLayout->setSizeConstraint(QLayout::SetMaximumSize);
        _transporterLayout->addWidget(_menuButton);
        _transporterLayout->addWidget(_runButton);
        _transporterLayout->addWidget(_replayButton);
        _transporterLayout->addWidget(_breakButton);
        _transporterLayout->addStretch();
    }
}

void MainWindow::BindShortcuts()
{
    //_doMenu->setShortcut(QKeySequence("Esc"));

    _doRunScript->setShortcut(QKeySequence("F5"));
    _doReplayLast->setShortcut(QKeySequence("F6"));
    _doBreak->setShortcut(QKeySequence("F8"));

    _doOpenProject->setShortcut(QKeySequence("Ctrl+O"));
    _doReloadProject->setShortcut(QKeySequence("Ctrl+R"));
    _doCloseProject->setShortcut(QKeySequence("Ctrl+Q"));

    _doCloseCurrentFile->setShortcut(QKeySequence("Ctrl+T"));
    _doSaveCurrentFile->setShortcut(QKeySequence("Ctrl+S"));
    _doSaveCurrentFileAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
    _doSaveAllFiles->setShortcut(QKeySequence("Ctrl+D"));

    _doToggleFullscreen->setShortcut(QKeySequence("F11"));
    _doToggleOutputPanel->setShortcut(QKeySequence("F10"));
    _doToggleProjectTree->setShortcut(QKeySequence("F9"));
    _doTogglePlotter->setShortcut(QKeySequence("F12"));

    //_doShowPreferences->setShortcut(QKeySequence("F10"));
    //_doShowHelp->setShortcut(QKeySequence("F10"));

    _doNyquistRefresh->setShortcut(QKeySequence("Ctrl+Shift+R"));
    _doNyquistClearOutput->setShortcut(QKeySequence("Ctrl+Shift+E"));
    _doLogClear->setShortcut(QKeySequence("Ctrl+Shift+L"));

    _doQuit->setShortcut(QKeySequence("Alt+F4"));

    _doTest->setShortcut(QKeySequence("F1"));
}

void MainWindow::BindSlots()
{
    connect(_doMenu,       SIGNAL(triggered(bool)), this, SLOT(onMenu()));
    connect(_doRunScript,  SIGNAL(triggered(bool)), this, SLOT(onGo()));
    connect(_doReplayLast, SIGNAL(triggered(bool)), this, SLOT(onReplayLast()));
    connect(_doBreak,      SIGNAL(triggered(bool)), this, SLOT(onBreak()));

    connect(_doOpenProject,       SIGNAL(triggered(bool)), this, SLOT(onOpenFolder()));
    connect(_doReloadProject,     SIGNAL(triggered(bool)), this, SLOT(onReloadProject()));
    //connect(_doCloseProject,      SIGNAL(triggered(bool)), this, SLOT());
    connect(_doCloseCurrentFile,  SIGNAL(triggered(bool)), this, SLOT(onCloseCurrentFile()));
    connect(_doSaveCurrentFile,   SIGNAL(triggered(bool)), this, SLOT(onSaveCurrentFile()));
    connect(_doSaveCurrentFileAs, SIGNAL(triggered(bool)), this, SLOT(onSaveCurrentFileAs()));
    connect(_doSaveAllFiles,      SIGNAL(triggered(bool)), this, SLOT(onSaveAllFiles()));

    connect(_doToggleFullscreen,   SIGNAL(triggered(bool)), this, SLOT(onFullscreen()));
    connect(_doToggleOutputPanel,  SIGNAL(triggered(bool)), this, SLOT(onToggleOutput()));
    connect(_doToggleProjectTree,  SIGNAL(triggered(bool)), this, SLOT(onToggleProjectTree()));
    connect(_doTogglePlotter,      SIGNAL(triggered(bool)), this, SLOT(onTogglePlotter()));
    //connect(_doShowPreferences,    SIGNAL(triggered(bool)), this, SLOT());
    //connect(_doShowHelp,           SIGNAL(triggered(bool)), this, SLOT());
    connect(_doNyquistRefresh,     SIGNAL(triggered(bool)), this, SLOT(onRefresh()));
    connect(_doNyquistClearOutput, SIGNAL(triggered(bool)), this, SLOT(onClear()));
    //connect(_doLogClear,           SIGNAL(triggered(bool)), this, SLOT());

    connect(_doQuit, SIGNAL(triggered(bool)), this, SLOT(onQuit()));
    connect(_doTest, SIGNAL(triggered(bool)), this, SLOT(onTest()));
}

void MainWindow::BindActions()
{
    addAction(_doMenu);
    addAction(_doRunScript);
    addAction(_doReplayLast);
    addAction(_doBreak);
    addAction(_doOpenProject);
    addAction(_doReloadProject);
    addAction(_doCloseProject);
    addAction(_doCloseCurrentFile);
    addAction(_doSaveCurrentFile);
    addAction(_doSaveCurrentFileAs);
    addAction(_doSaveAllFiles);
    addAction(_doToggleFullscreen);
    addAction(_doToggleOutputPanel);
    addAction(_doToggleProjectTree);
    addAction(_doTogglePlotter);
    addAction(_doShowPreferences);
    addAction(_doShowHelp);
    addAction(_doNyquistRefresh);
    addAction(_doNyquistClearOutput);
    addAction(_doLogClear);
    addAction(_doQuit);
    addAction(_doTest);
}


void MainWindow::SetupTheme()
{
    Styler styler;
    setStyleSheet(styler.CssMain());
}

void MainWindow::SetButtonGlyph(QPushButton *button, QString glyphPath)
{
    if(button){
        button->setIcon(Storage::getInstance().icon(glyphPath));
        button->setIconSize(QSize(45,45));
        button->setText("");
    }
}

void MainWindow::onTogglePlotter()
{
    _canvas->setVisible(!_canvas->isVisible());
}

//
//
// Events
//
//

void MainWindow::closeEvent(QCloseEvent *event){
    _allowQuit = false;
    onQuitApplication();
    if (!_allowQuit){event->ignore();}
}

//
//
// Slots
//
//

void MainWindow::onStdoutAvailable(){
    Logger::Write("Nyquist output");
    QByteArray data = _controller.GetNyquistProcess()->readAllStandardOutput();
    QString received(data);
    CheckOutput(received);
    ui->outputNyquistArea->append(received);
}

void MainWindow::onFinished(int, QProcess::ExitStatus){}

void MainWindow::CheckOutput(QString data)
{
    _analyzer->Analyze(data);
}

void MainWindow::OpenNewTab(const QModelIndex &index)
{

    NyEditor *page = new NyEditor(ui->editorMain);
    page->SetPrompter(_promptr);

    int idx = ui->editorMain->addTab(page, "");
    page->SetContext(idx, ui->editorMain);
    page->setFocus();
    ui->editorMain->setCurrentIndex(idx);

    if (index.isValid()){
        NyProjectItem *item = static_cast<NyProjectItem*>(index.internalPointer());
        item->Open(true, page);
    }
}

void MainWindow::onGo()
{
    Logger::Write("Run script");
    NyEditor *e = (NyEditor*)ui->editorMain->currentWidget();
    if (!e) return;

    QString localSrc = _project->ScriptToRun(e);
    if ("" == localSrc) return;

    Logger::Write(localSrc);
    if (e->document()->isModified()){
        _project->SaveEdited(e);
    }

    _controller.ExecuteFile(localSrc);
}

void MainWindow::onReplayLast()
{
    _controller.Replay();
}

void MainWindow::onBreak()
{
    _controller.Break();
}

void MainWindow::onMenu()
{
    if (_menuButton){
        QPoint p(
            _menuButton->pos().x() + 2,
            _menuButton->pos().y() + _menuButton->height() + 2
        );
        ShowContextMenu(p);
    }
}

void MainWindow::onClear()
{
    ui->outputNyquistArea->clear();
}

void MainWindow::onRefresh()
{
    _controller.Restart();
}

void MainWindow::onTest()
{
    QString dir = "D:/Programy/Nyquist/jnyqide/Nyq";
    _project = new NyProjectModel(dir, this);

    ui->projectStructureView->setModel(_project);
    ui->projectStructureView->expandAll();

    _controller.SetupProject(dir);
}

void MainWindow::onFullscreen()
{
    _fullscreen = !_fullscreen;

    if (_fullscreen) {
        _maximized = isMaximized();
        showFullScreen();
    } else {
        if (_maximized) {
            showMaximized();
        } else {
            showNormal();
        }
    }
}

void MainWindow::onToggleOutput()
{
    ui->outputPane->setVisible(
        !ui->outputPane->isVisible()
                );
}

void MainWindow::onToggleProjectTree()
{
    ui->navigatorPane->setVisible(
        !ui->navigatorPane->isVisible()
                );
}

void MainWindow::onOpenFolder()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir != "")
    {
        _project = new NyProjectModel(dir, this);
        ui->projectStructureView->setModel(_project);
        ui->projectStructureView->expandAll();
        _controller.SetupProject(dir);
        Storage::getInstance().projectLoaded(true);
    }
}

void MainWindow::onPlot()
{
    _canvas->setVisible(true);
    _canvas->Plot(
                _analyzer->PointsPath(),
                _analyzer->SoundDuration()
             );
}

void MainWindow::onProjectElementSelection(const QModelIndex &index)
{
    NyProjectItem *sourceFile = static_cast<NyProjectItem*>(index.internalPointer());
    if (sourceFile){
        bool isFile = !sourceFile->IsDirectory();
        bool isReady = !sourceFile->IsOpen();

        if (isFile && isReady){
            OpenNewTab(index);
        }
    }

}

void MainWindow::onProjectMenuRequest(const QPoint &point)
{
    QModelIndex index = ui->projectStructureView->indexAt(point);
    if (index.isValid()) {
        NyProjectItem *item = static_cast<NyProjectItem*>(index.internalPointer());

        if (item->IsDirectory()){
            _doAddElement->setVisible(true);
            _doFoldNode->setVisible(true);
            _doUnfoldNode->setVisible(true);
        } else {
            _doAddElement->setVisible(false);
            _doFoldNode->setVisible(false);
            _doUnfoldNode->setVisible(false);
        }
        _projectTreeMenu->exec(ui->projectStructureView->mapToGlobal(point));
    }
}

void MainWindow::onReloadProject()
{
    if (_project && SaveAllFiles()){
        //_project->Reload(); //!!
    }
}

void MainWindow::onRenameNode()
{
    QString current = "";
    QModelIndex selected = ui->projectStructureView->currentIndex();
    current = _project->FileNameOfIndex(selected);
    NyNodeDialog *dialog = new NyNodeDialog();
    dialog->Set(current);
    dialog->exec();

    if (dialog->Response()){
        _project->RenameIndex(selected, dialog->Value());
    }
}

void MainWindow::onSaveCurrentFile()
{
    NyEditor *e = (NyEditor*)ui->editorMain->currentWidget();
    _project->SaveEdited(e);
}

void MainWindow::onSaveCurrentFileAs()
{
    NyEditor *e = (NyEditor*)ui->editorMain->currentWidget();
    QString name = QFileDialog::getSaveFileName(0, QObject::tr("Save file as..."), _project->RootPath(), QObject::tr("Lisp source files (*.lsp)"));

    _project->SaveEditedAs(name, e);
    ui->projectStructureView->expandAll();
}

void MainWindow::onSaveAllFiles()
{
    SaveAllFiles();
}

void MainWindow::onCloseCurrentFile()
{
    onCLoseTab(-1);
}

void MainWindow::onQuit()
{
    bool go = true;
    for (int i = 0; i < ui->editorMain->count(); i++){
        NyEditor *editor = (NyEditor*)ui->editorMain->widget(i);
        if (editor->document()->isModified()){

            int ret = QMessageBox::question(this, tr("Question"),
                                           tr("Some files are left unsaved.\n"
                                              "Do you want to save your changes?"),
                                           QMessageBox::Save | QMessageBox::Discard
                                           | QMessageBox::Cancel,
                                           QMessageBox::Save);
            switch (ret) {
            case QMessageBox::Save:
                go = SaveAllFiles();
                break;
            case QMessageBox::Cancel:
                go = false;
                break;
            default:
                break;
            }
        }
    }

    _allowQuit = go;

    if (go){
        _controller.Shutdown();
        exit(0);
    }
}

bool MainWindow::SaveAllFiles()
{
    return _project->SaveAll();
}

void MainWindow::onQuitApplication()
{
    onQuit();
}

//
//
// Menu
//
//

void MainWindow::ShowContextMenu(const QPoint &pos)
{
    if(_mainMenu)
    {
        _mainMenu->exec(mapToGlobal(pos));
    }
}

void MainWindow::BuildMenu()
{
    _mainMenu = new QMenu(tr("Nyquist Coder Menu"), this);

    // this is 'quick access' part, will be build from configuration
    // only in button mode
    _mainMenu->addAction(_doOpenProject);
    _mainMenu->addSeparator();

    _projectMenu = _mainMenu->addMenu(tr("Project"));
    _projectMenu->addAction(_doOpenProject);
    _projectMenu->addAction(_doReloadProject);
    _projectMenu->addAction(_doCloseProject);
    _projectMenu->addSeparator();

    _projectMenu->addAction(_doCloseCurrentFile);
    _projectMenu->addAction(_doSaveCurrentFile);
    _projectMenu->addAction(_doSaveCurrentFileAs);
    _projectMenu->addAction(_doSaveAllFiles);

    _viewMenu = _mainMenu->addMenu(tr("View"));
    _viewMenu->addAction(_doToggleOutputPanel);
    _viewMenu->addAction(_doToggleProjectTree);
    _viewMenu->addAction(_doTogglePlotter);
    _viewMenu->addSeparator();
    _viewMenu->addAction(_doToggleFullscreen);

    _nyquistMenu = _mainMenu->addMenu(tr("Nyquist"));
    _nyquistMenu->addAction(_doRunScript);
    _nyquistMenu->addAction(_doReplayLast);
    _nyquistMenu->addAction(_doBreak);
    _nyquistMenu->addSeparator();

    _nyquistMenu->addAction(_doNyquistRefresh);
    _nyquistMenu->addAction(_doNyquistClearOutput);

    _mainMenu->addSeparator();

    _mainMenu->addAction(_doShowPreferences);
    _mainMenu->addAction(_doShowHelp);
    _mainMenu->addSeparator();

    _mainMenu->addAction(_doQuit);

    _mainMenu->addAction(_doTest);
}

void MainWindow::BuildProjectMenu()
{
    ui->projectStructureView->setContextMenuPolicy(Qt::CustomContextMenu);
    _projectTreeMenu = new QMenu(ui->projectStructureView);
    _projectTreeMenu->addAction(_doAddElement);
    _projectTreeMenu->addAction(_doRenameElement);
    _projectTreeMenu->addAction(_doRemoveElement);
    _projectTreeMenu->addAction(_doFoldNode);
    _projectTreeMenu->addAction(_doUnfoldNode);

    connect(_doRenameElement, SIGNAL(triggered(bool)), this, SLOT(onRenameNode()));
    connect(
        ui->projectStructureView,
        SIGNAL(customContextMenuRequested(const QPoint &)), this,
        SLOT(onProjectMenuRequest(QPoint))
    );
}

void MainWindow::BuildWorkspace()
{
    CreateTransportButtons();
    CreatePlotterCanvas();

    _workspaceSplitter = new QSplitter(this);
    _workspaceSplitter->setOrientation(Qt::Horizontal);

    _mainSplitter = new QSplitter(this);
    _mainSplitter->setOrientation(Qt::Vertical);

    _mainSplitter->addWidget(_workspaceSplitter);
    _mainSplitter->addWidget(_canvas);

    _workspaceSplitter->addWidget(ui->navigatorPane);
    _workspaceSplitter->addWidget(ui->editorPane);
    _workspaceSplitter->addWidget(ui->outputPane);

    setCentralWidget(_mainSplitter);
}

void MainWindow::DestroyMenuNodes()
{
    delete _projectMenu;
    delete _viewMenu;
    delete _nyquistMenu;

    delete _mainMenu;
}

//
//
// Project management
//
//


//
//
// Editor tabs
//
//

void MainWindow::onCLoseTab(int index)
{
    bool go = true;

    NyEditor *e = (NyEditor*)ui->editorMain->currentWidget();
    int indx = ui->editorMain->currentIndex();

    if (indx != index && index >= 0){
        indx = index;
    }

    if (e->document()->isModified()) {

        int ret = QMessageBox::question(this, tr("Question"),
                                       tr("The document has been modified.\n"
                                          "Do you want to save your changes?"),
                                       QMessageBox::Save | QMessageBox::Discard
                                       | QMessageBox::Cancel,
                                       QMessageBox::Save);
        switch (ret) {
        case QMessageBox::Save:
            //go = e->Save();
            break;
        case QMessageBox::Cancel:
            go = false;
            break;
        default:
            break;
        }
    }

    if (go){
        ui->editorMain->removeTab(indx);
        _project->CloseEdited(e);
    }
}

void MainWindow::CreateActions()
{
    _doMenu = new QAction("Main menu", this);
    _doRunScript = new QAction("Run", this);
    _doReplayLast = new QAction("Replay", this);
    _doBreak = new QAction("Break", this);

    _doOpenProject = new QAction("Open project", this);
    _doReloadProject = new QAction("Reload project", this);
    _doCloseProject = new QAction("Close project", this);

    _doCloseCurrentFile = new QAction("Close current file", this);
    _doSaveCurrentFile = new QAction("Save current file", this);
    _doSaveCurrentFileAs = new QAction("Save current file as...", this);
    _doSaveAllFiles = new QAction("Save all files", this);

    _doToggleFullscreen = new QAction("Toggle fullscreen", this);
    _doToggleOutputPanel = new QAction("Toggle output panel", this);
    _doToggleProjectTree = new QAction("Toggle project tree", this);
    _doTogglePlotter = new QAction("Toggle sound plotter", this);

    _doShowPreferences = new QAction("Preferences", this);
    _doShowHelp = new QAction("Help", this);

    _doNyquistRefresh = new QAction("Refresh Nyquist", this);
    _doNyquistClearOutput = new QAction("Clear Nyquist output", this);

    _doLogClear = new QAction("Clear log output", this);

    _doQuit = new QAction("Quit", this);

    _doTest = new QAction("Test", this);

    _doAddElement = new QAction("Add", this);
    _doRenameElement = new QAction("Rename", this);
    _doRemoveElement = new QAction("Remove", this);
    _doFoldNode = new QAction("Fold", this);
    _doUnfoldNode = new QAction("Unfold", this);


    BindShortcuts();
    BindSlots();
    BindActions();
}

void MainWindow::DestroyActions()
{
    delete _doMenu;
    delete _doRunScript;
    delete _doReplayLast;
    delete _doBreak;

    delete _doOpenProject;
    delete _doReloadProject;
    delete _doCloseProject;

    delete _doCloseCurrentFile;
    delete _doSaveCurrentFile;
    delete _doSaveCurrentFileAs;
    delete _doSaveAllFiles;

    delete _doToggleFullscreen;
    delete _doToggleOutputPanel;
    delete _doToggleProjectTree;
    delete _doTogglePlotter;

    delete _doShowPreferences;
    delete _doShowHelp;

    delete _doNyquistRefresh;
    delete _doNyquistClearOutput;

    delete _doLogClear;

    delete _doQuit;
    delete _doTest;

    delete _doAddElement;
    delete _doRenameElement;
    delete _doRemoveElement;
    delete _doFoldNode;
    delete _doUnfoldNode;

}
