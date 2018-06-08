#include "mainwindow.h"
#include <ui_mainwindow.h>

#include "projectitem.h"
#include "storage.h"
#include "styler.h"

#include <fstream>
#include <iostream>

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include <QCloseEvent>


#include <src/editor/editor.h>

#include <src/storage/storagefile.h>

//
//
// Lifetime
//
//

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

    _controller.Start();

    qDebug() << Storage::getInstance().lispWordsRegEx();
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
    mainSplitter = new QSplitter(this);
    mainSplitter->addWidget(ui->navigatorPane);
    mainSplitter->addWidget(ui->editorPane);
    mainSplitter->addWidget(ui->outputPane);
    setCentralWidget(mainSplitter);
    showMaximized();
}

void MainWindow::EditorsSettings()
{
    ui->editorMain->setTabsClosable(true);
    ui->editorAdditional->setTabsClosable(true);
    ui->editorAdditional->hide();
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

void MainWindow::SetupTheme()
{
    Styler styler;
    setStyleSheet(styler.CssMain());
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
    std::cout << "Save current";
}

void MainWindow::onSaveAllFiles()
{
    std::cout << "Save all";
}

void MainWindow::onQuitApplication()
{
    _controller.Shutdown();
}

void MainWindow::onProjectElementSelection(QTreeWidgetItem *item, int column)
{
    ProjectItem *sourceFile = dynamic_cast<ProjectItem*>(item);
    if (sourceFile){
        Editor *page = new Editor(this, sourceFile->getFilePath());
        ui->editorMain->addTab(page, sourceFile->getFileName());
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
    mainMenu = new QMenu(tr("NyCoder"), this);

    miOpenFolder      = new QAction("Open folder", this);
    miSaveCurrentFile = new QAction("Save current file", this);
    miSaveAllFiles    = new QAction("Save all files", this);
    miQuitApplication = new QAction("Quit", this);
    miRunCurrentFile  = new QAction("Run", this);
    miTest            = new QAction("Test", this);

    mainMenu->addAction(miOpenFolder);
    mainMenu->addAction(miSaveCurrentFile);
    mainMenu->addAction(miSaveAllFiles);
    mainMenu->addAction(miRunCurrentFile);
    mainMenu->addAction(miQuitApplication);
    mainMenu->addAction(miTest);

    BindMenuItemsWithSlots();
}

void MainWindow::BindMenuItemsWithSlots()
{
    connect(miOpenFolder, SIGNAL(triggered(bool)), this, SLOT(onOpenFolder()));
    connect(miRunCurrentFile, SIGNAL(triggered(bool)), this, SLOT(onGo()));
    connect(miTest, SIGNAL(triggered(bool)), this, SLOT(onTest()));
}

void MainWindow::DestroyMenuItems()
{
    delete miOpenFolder;
    delete miSaveCurrentFile;
    delete miSaveAllFiles;
    delete miQuitApplication;
    delete miRunCurrentFile;

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
    Editor *e = (Editor*)ui->editorMain->currentWidget();
    if (e->document()->isModified()) {

        int ret = QMessageBox::question(this, tr("Question"),
                                       tr("The document has been modified.\n"
                                          "Do you want to save your changes?"),
                                       QMessageBox::Save | QMessageBox::Discard
                                       | QMessageBox::Cancel,
                                       QMessageBox::Save);
        bool go = false;
        switch (ret) {
        case QMessageBox::Save:
            go = e->Save();
            break;
        case QMessageBox::Discard:
            go = true;
            break;
        default:
            break;
        }
        if (go){
            ui->editorMain->removeTab(index);
        }
    }
}
