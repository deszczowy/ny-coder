#include "mainwindow.h"
#include <ui_mainwindow.h>

#include "projecttree.h"
#include "projectitem.h"

#include <fstream>
#include <iostream>

#include <QDebug>
#include <QFileDialog>

#include <QCloseEvent>


#include <src/editor/editor.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //
    mainSplitter = new QSplitter(this);
    mainSplitter->addWidget(ui->navigatorPane);
    mainSplitter->addWidget(ui->editorPane);
    mainSplitter->addWidget(ui->outputPane);
    setCentralWidget(mainSplitter);

    ui->editorMain->setTabsClosable(true);
    ui->editorAdditional->setTabsClosable(true);
    ui->editorAdditional->hide();

BuildMenuActionsStructure();

    connect(_controller.GetNyquistProcess(), SIGNAL(readyReadStandardOutput()), this, SLOT(onStdoutAvailable()) );
    connect(_controller.GetNyquistProcess(), SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)) );

ui->outputArea->setFont(QFont("courier"));

    _controller.Start();

    showMaximized();
}

MainWindow::~MainWindow()
{
    DestroyMenuItems();
    delete ui;
}



void MainWindow::closeEvent(QCloseEvent *event){
    onQuitApplication();
}

void MainWindow::onStdoutAvailable(){
    QByteArray data = _controller.GetNyquistProcess()->readAllStandardOutput();
    ui->outputArea->append(QString(data));
}
void MainWindow::onFinished(int, QProcess::ExitStatus){}

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

    mainMenu->addAction(miOpenFolder);
    mainMenu->addAction(miSaveCurrentFile);
    mainMenu->addAction(miSaveAllFiles);
    mainMenu->addAction(miRunCurrentFile);
    mainMenu->addAction(miQuitApplication);

    BindMenuItemsWithSlots();
}

void MainWindow::BindMenuItemsWithSlots()
{
    connect(miOpenFolder, SIGNAL(triggered(bool)), this, SLOT(onOpenFolder()));
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











void MainWindow::on_goButton_clicked()
{
    /*
        QString localSrc =
        ((Editor*)ui->editorMain->currentWidget())->Path();

        std::string ba = ((Editor*)ui->editorMain->currentWidget())->GetContent().toStdString();



        _controller.ExecuteFile(localSrc);
        */
}





void MainWindow::on_breakButton_clicked()
{

}

void MainWindow::onOpenFolder()
{
    QStringList extensions;
    extensions << ".lsp" << ".lisp";

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ProjectTree tree(ui->projectStructureView, dir, extensions);
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

void MainWindow::onRunCurrentFile()
{
    std::cout << "Run";
}

void MainWindow::on_projectStructureView_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ProjectItem *sourceFile = dynamic_cast<ProjectItem*>(item);
    if (sourceFile){
        /*
        EditorPage *page = new EditorPage;
        page->loadFile(sourceFile->getFilePath());
        ui->editorMain->addTab(page, sourceFile->getFileName());
        */
        Editor *page = new Editor(this, sourceFile->getFilePath());
        ui->editorMain->addTab(page, sourceFile->getFileName());
    }
}

void MainWindow::on_clearOutput_clicked()
{
    ui->outputArea->clear();
}

void MainWindow::on_refreshOutput_clicked()
{
    ui->outputArea->clear();

}

void MainWindow::on_menuButton_clicked()
{
    QPoint p(
        ui->menuButton->pos().x() + 2,
        ui->menuButton->pos().y() + ui->menuButton->height() + 2
    );
    ShowContextMenu(p);
}
