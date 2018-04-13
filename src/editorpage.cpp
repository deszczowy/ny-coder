#include "editorpage.h"
#include "ui_editorpage.h"

#include "QFile"
#include "QTextStream"

EditorPage::EditorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorPage)
{
    ui->setupUi(this);
}

EditorPage::~EditorPage()
{
    delete ui;
}

void EditorPage::loadFile(QString path)
{
    _path = path;

    QFile ffile(_path);
    if (ffile.open(QFile::ReadOnly | QIODevice::Text)) {
        QTextStream in(&ffile);
        ui->Editor->setText(in.readAll());
    }
}

QString EditorPage::getPath()
{
    return _path;
}

QString EditorPage::getContent()
{
    return ui->Editor->toPlainText();
}
