#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editorpage.h"
#include "projecttree.h"

#include "QProcess"
#include "QDebug"
#include "QFileDialog"

#include <QCloseEvent>

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
        QFile ffile("test.lsp");
        if (ffile.open(QFile::WriteOnly | QIODevice::Text)) {
            QTextStream out(&ffile);
            //out << ui->editorMain->toPlainText();
        }

        process->write("(load \"test.lsp\") \n");
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

void MainWindow::on_projectStructureView_doubleClicked(const QModelIndex &index)
{/*
    //QModelIndex index = ui->projectStructureView->currentIndex();
    QString path = ((QFileSystemModel)ui->projectStructureView->model()).filePath(index);
    QFile file(path);

    file.open(QFile::ReadOnly | QFile::Text);

    QTextStream ReadFile(&file);
    //ui->tabEditor1->setText(ReadFile.readAll());
    */
}

void MainWindow::on_breakButton_clicked()
{
    EditorPage *page = new EditorPage;
    ui->editorMain->addTab(page, tr("General"));
}

void MainWindow::onOpenFolder()
{
    QStringList extensions;
    extensions << ".lsp" << ".lisp";

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ProjectTree tree(ui->projectStructureView, dir, extensions);
}
