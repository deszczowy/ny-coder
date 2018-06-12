#include "mainwindow.h"
#include <ui_mainwindow.h>

#include "preferences.h"

#include "projectitem.h"
#include "storage.h"
#include "styler.h"

#include <fstream>
#include <iostream>

#include <QDebug>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QShortcut>

#include <QCloseEvent>


#include <src/editor/editor.h>

#include <src/storage/storagefile.h>

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
    BaseUiSettings();
    SetupTheme();
    EditorsSettings();
    OutputSettings();
    ConnectSlots();
    BuildMenuActionsStructure();
    BindShortcuts();

    OpenNewTab("New", ""); //
    _controller.Start();
}

MainWindow::~MainWindow()
{
    DestroyMenuItems();
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

    _fullscreen = false;

    SetButtonGlyph(ui->breakButton, ":/icon/res/ico/stop-script-normal.png");
    SetButtonGlyph(ui->goButton, ":/icon/res/ico/run-script-normal.png");
    SetButtonGlyph(ui->menuButton, ":/icon/res/ico/menu-normal.png");

    mainSplitter = new QSplitter(this);
    mainSplitter->addWidget(ui->navigatorPane);
    mainSplitter->addWidget(ui->editorPane);
    mainSplitter->addWidget(ui->outputPane);

    setCentralWidget(mainSplitter);

    setWindowTitle("Nyquist Coder :: version 0.0");

    showMaximized();
    _maximized = true;
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
    ui->outputArea->setFont(QFont("Courier", 12, 1));
}

void MainWindow::ConnectSlots()
{
    // nyquist output
    connect(_controller.GetNyquistProcess(), SIGNAL(readyReadStandardOutput()), this, SLOT(onStdoutAvailable()) );
    connect(_controller.GetNyquistProcess(), SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)) );

    // transport buttons
    connect(ui->goButton, SIGNAL(clicked(bool)), this, SLOT(onGo()) );
    connect(ui->breakButton, SIGNAL(clicked(bool)), this, SLOT(onBreak()) );
    connect(ui->menuButton, SIGNAL(clicked(bool)), this, SLOT(onMenu()) );

    // output buttons
    connect(ui->clearOutput, SIGNAL(clicked(bool)), this, SLOT(onClear()) );
    connect(ui->refreshOutput, SIGNAL(clicked(bool)), this, SLOT(onRefresh()) );

    // editors
    connect(ui->editorMain, SIGNAL(tabCloseRequested(int)), this, SLOT(onCLoseTab(int)));

    // project
    connect(ui->projectStructureView, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onProjectElementSelection(QTreeWidgetItem*,int)) );
}

void MainWindow::BindShortcuts()
{
    new QShortcut(QKeySequence("F5"), this, SLOT(onGo()));
    new QShortcut(QKeySequence("F1"), this, SLOT(onTest()));
    new QShortcut(QKeySequence("F11"), this, SLOT(onFullscreen()));
    new QShortcut(QKeySequence("F10"), this, SLOT(onBreak()));
    new QShortcut(QKeySequence("Ctrl+S"), this, SLOT(onSaveCurrentFile()));
    new QShortcut(QKeySequence("Ctrl+Shift+S"), this, SLOT(onSaveAllFiles()));
    new QShortcut(QKeySequence("Esc"), this, SLOT(onMenu()));
}

void MainWindow::SetupTheme()
{
    Styler styler;
    setStyleSheet(styler.CssMain());
}

void MainWindow::SetButtonGlyph(QPushButton *button, QString glyphPath)
{
    if(button){
        button->setIcon(QIcon(glyphPath));
        button->setIconSize(QSize(45,45));
        button->setText("");
    }
}

//
//
// Events
//
//

void MainWindow::closeEvent(QCloseEvent *event){
    onQuitApplication();
}

//
//
// Slots
//
//

void MainWindow::onStdoutAvailable(){
    QByteArray data = _controller.GetNyquistProcess()->readAllStandardOutput();
    ui->outputArea->append(QString(data));
}

void MainWindow::onFinished(int, QProcess::ExitStatus){}

void MainWindow::OpenNewTab(QString fileName, QString path)
{
    Editor *page = new Editor(ui->editorMain, path);
    int idx = ui->editorMain->addTab(page, fileName);
    page->SetContext(idx, ui->editorMain);
    page->setFocus();
    page->document()->setModified(false);
    ui->editorMain->setCurrentIndex(idx);
}

void MainWindow::onGo()
{
    Editor *e = (Editor*)ui->editorMain->currentWidget();
    bool canGo = true;

    if (e)
    {
        if (e->document()->isModified()){
            canGo = e->Save();
        }

        if (canGo)
        {
            QString localSrc = (e)->Path();
            if (localSrc != "")
            {
                _controller.ExecuteFile(localSrc);
            }
        }
    }
}

void MainWindow::onBreak()
{
    _controller.Break();
}

void MainWindow::onMenu()
{
    QPoint p(
        ui->menuButton->pos().x() + 2,
        ui->menuButton->pos().y() + ui->menuButton->height() + 2
    );
    ShowContextMenu(p);
}

void MainWindow::onClear()
{
    ui->outputArea->clear();
}

void MainWindow::onRefresh()
{
    _controller.Restart();
}

void MainWindow::onTest()
{
    QStringList extensions;
    extensions << ".lsp" << ".lisp";
    ui->projectStructureView->clear();
    _project = new ProjectTree(ui->projectStructureView, "D:\\Programy\\Nyquist\\jnyqide\\Nyq", extensions);
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
    QStringList extensions;
    extensions << ".lsp" << ".lisp";

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir != "")
    {
        ui->projectStructureView->clear();
        _project = new ProjectTree(ui->projectStructureView, dir, extensions);
    }
}

void MainWindow::onSaveCurrentFile()
{
    Editor *e = (Editor*)ui->editorMain->currentWidget();
    e->Save();
}

void MainWindow::onSaveAllFiles()
{
    bool go = true;
    for (int i = 0; i < ui->editorMain->count(); i++){
        Editor *editor = (Editor*)ui->editorMain->widget(i);
        if (go){
            go = editor->Save();
        }
    }
}

void MainWindow::onQuitApplication()
{
    _controller.Shutdown();
}

void MainWindow::onProjectElementSelection(QTreeWidgetItem *item, int column)
{
    ProjectItem *sourceFile = dynamic_cast<ProjectItem*>(item);
    if (sourceFile){
        OpenNewTab(
                    sourceFile->getFileName(),
                    sourceFile->getFilePath()
        );
    }
}

//
//
// Menu
//
//

void MainWindow::ShowContextMenu(const QPoint &pos)
{
    if(mainMenu)
    {
        mainMenu->exec(mapToGlobal(pos));
    }
}

void MainWindow::BuildMenuActionsStructure()
{
    // Actions

    // projectMenu
    miOpenFolder        = new QAction("Open folder", this);
    miReloadProject     = new QAction("Reload project", this);
    miSaveCurrentFile   = new QAction("Save current file", this);
    miSaveCurrentFileAs = new QAction("Save current file as...", this);
    miSaveAllFiles      = new QAction("Save all files", this);
    miCloseCurrentFile  = new QAction("Close current file", this);
    miCloseProject      = new QAction("Close project", this);

    // viewMenu
    miSwitchNyquistOutput    = new QAction("Toggle Nyquist output", this);
    miSwitchProjectStructure = new QAction("Toggle project structure", this);
    miFullscreen             = new QAction("Toggle fullscreen", this);

    // nyquistMenu
    miRunCurrentFile = new QAction("Run", this);
    miReplay         = new QAction("Replay", this);
    miBreak          = new QAction("Break", this);
    miRefreshNyquist = new QAction("Refresh Nyquist", this);
    miClearOutput    = new QAction("Clear output", this);

    // main menu
    miPreferences     = new QAction("Preferences", this);
    miQuitApplication = new QAction("Quit", this);

    // debug
    miTest = new QAction("Test", this);

    // Structure

    mainMenu = new QMenu(tr("Nyquist Coder Menu"), this);

    // this will be build from configuration
    mainMenu->addAction(miOpenFolder);

    mainMenu->addSeparator();

    projectMenu = mainMenu->addMenu(tr("Project"));
    projectMenu->addAction(miOpenFolder);
    projectMenu->addAction(miReloadProject);
    projectMenu->addSeparator();
    projectMenu->addAction(miSaveCurrentFile);
    projectMenu->addAction(miSaveCurrentFileAs);
    projectMenu->addAction(miSaveAllFiles);
    projectMenu->addSeparator();
    projectMenu->addAction(miCloseCurrentFile);
    projectMenu->addAction(miCloseProject);

    viewMenu = mainMenu->addMenu(tr("View"));
    viewMenu->addAction(miSwitchNyquistOutput);
    viewMenu->addAction(miSwitchProjectStructure);
    viewMenu->addSeparator();
    viewMenu->addAction(miFullscreen);

    nyquistMenu = mainMenu->addMenu(tr("Nyquist"));
    nyquistMenu->addAction(miRunCurrentFile);
    nyquistMenu->addAction(miReplay);
    nyquistMenu->addAction(miBreak);
    nyquistMenu->addSeparator();
    nyquistMenu->addAction(miRefreshNyquist);
    nyquistMenu->addAction(miClearOutput);

    mainMenu->addSeparator();
    mainMenu->addAction(miPreferences);
    mainMenu->addSeparator();
    mainMenu->addAction(miQuitApplication);

    mainMenu->addAction(miTest);

    // bindings

    BindMenuItemsWithSlots();
}

void MainWindow::BindMenuItemsWithSlots()
{
    connect(miOpenFolder, SIGNAL(triggered(bool)), this, SLOT(onOpenFolder()));
    connect(miRunCurrentFile, SIGNAL(triggered(bool)), this, SLOT(onGo()));
    connect(miSaveAllFiles, SIGNAL(triggered(bool)), this, SLOT(onSaveAllFiles()));
    connect(miSaveCurrentFile, SIGNAL(triggered(bool)), this, SLOT(onSaveCurrentFile()));
    connect(miQuitApplication, SIGNAL(triggered(bool)), this, SLOT(onQuitApplication()));
    connect(miSwitchNyquistOutput, SIGNAL(triggered(bool)), this, SLOT(onToggleOutput()));
    connect(miSwitchProjectStructure, SIGNAL(triggered(bool)), this, SLOT(onToggleProjectTree()));
    connect(miTest, SIGNAL(triggered(bool)), this, SLOT(onTest()));
}

void MainWindow::DestroyMenuItems()
{
    delete miOpenFolder;
    delete miReloadProject;
    delete miSaveCurrentFile;
    delete miSaveCurrentFileAs;
    delete miSaveAllFiles;
    delete miCloseCurrentFile;
    delete miCloseProject;
    delete miSwitchNyquistOutput;
    delete miSwitchProjectStructure;
    delete miFullscreen;
    delete miRunCurrentFile;
    delete miReplay;
    delete miBreak;
    delete miRefreshNyquist;
    delete miClearOutput;
    delete miPreferences;
    delete miQuitApplication;
    delete miTest;

    delete projectMenu;
    delete viewMenu;
    delete nyquistMenu;

    delete mainMenu;
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

    Editor *e = (Editor*)ui->editorMain->currentWidget();
    if (e->document()->isModified()) {

        int ret = QMessageBox::question(this, tr("Question"),
                                       tr("The document has been modified.\n"
                                          "Do you want to save your changes?"),
                                       QMessageBox::Save | QMessageBox::Discard
                                       | QMessageBox::Cancel,
                                       QMessageBox::Save);
        switch (ret) {
        case QMessageBox::Save:
            go = e->Save();
            break;
        case QMessageBox::Cancel:
            go = false;
            break;
        default:
            break;
        }
    }

    if (go){
        ui->editorMain->removeTab(index);
    }
}
