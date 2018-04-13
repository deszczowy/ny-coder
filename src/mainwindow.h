#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QProcess"
#include "QTextEdit"
#include "QSplitter"
#include "QTreeWidgetItem"

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

    void onOpenFolder();

    void on_projectStructureView_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_clearOutput_clicked();

    void on_refreshOutput_clicked();

private:
    Ui::MainWindow *ui;
    QProcess *process;
    QSplitter *mainSplitter;
    QSplitter *editorSplitter;

    bool nyquistIsRunning;

    void on_application_quit();

    void runNyquist();
    void exitNyquist();
};

#endif // MAINWINDOW_H
