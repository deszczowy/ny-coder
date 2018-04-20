#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "projecttree.h"
#include "projectitem.h"

#include "fstream"

#include "QProcess"
#include "QDebug"
#include "QFileDialog"

#include <QCloseEvent>

#include <src/editor/editor.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    process = new QProcess(this);
    //
    mainSplitter = new QSplitter(this);
    mainSplitter->addWidget(ui->navigatorPane);
    mainSplitter->addWidget(ui->editorPane);
    mainSplitter->addWidget(ui->outputPane);
    setCentralWidget(mainSplitter);

    ui->editorMain->setTabsClosable(true);
    ui->editorAdditional->setTabsClosable(true);
    ui->editorAdditional->hide();

    nyquistIsRunning = false;
    runNyquist();

    process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(onStdoutAvailable()) );
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)) );

    connect(ui->actionOpen_folder, SIGNAL(triggered(bool)), this, SLOT(onOpenFolder()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event){
    on_application_quit();
}

void MainWindow::onStdoutAvailable(){
    QByteArray data = process->readAllStandardOutput();
    ui->outputArea->append(QString(data));
}
void MainWindow::onFinished(int, QProcess::ExitStatus){}

void MainWindow::on_application_quit(){
    exitNyquist();
}

void MainWindow::on_goButton_clicked()
{
    if (nyquistIsRunning){
        QString localSrc =
        ((Editor*)ui->editorMain->currentWidget())->GetPath();

        std::string ba = ((Editor*)ui->editorMain->currentWidget())->GetContent().toStdString();

        std::ofstream out(localSrc.toStdString());
        out << ba;
        out.close();

        QString command("(load \"" + localSrc + "\") \n");
        process->write(command.toLatin1());

    }
}

void MainWindow::runNyquist(){
    process->start("ny", QIODevice::ReadWrite);
    process->write("(play (osc 50)) \n");
    nyquistIsRunning = true;
}

void MainWindow::exitNyquist(){
    if (nyquistIsRunning){
        process->write("(break) \n");
        process->write("(exit) \n");
        process->close();
    }
}

void MainWindow::on_breakButton_clicked()
{
    if (nyquistIsRunning){
        process->write("(top) \n");
    }
}

void MainWindow::onOpenFolder()
{
    QStringList extensions;
    extensions << ".lsp" << ".lisp";

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ProjectTree tree(ui->projectStructureView, dir, extensions);
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
        Editor *page = new Editor;
        page->LoadFile(sourceFile->getFilePath());
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
    exitNyquist();
    runNyquist();
}
