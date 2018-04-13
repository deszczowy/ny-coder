#ifndef EDITORPAGE_H
#define EDITORPAGE_H

#include <QWidget>

namespace Ui {
class EditorPage;
}

class EditorPage : public QWidget
{
    Q_OBJECT

public:
    explicit EditorPage(QWidget *parent = 0);
    ~EditorPage();

    void loadFile(QString path);
    QString getPath();
    QString getContent();

private:
    Ui::EditorPage *ui;

    QString _path;
};

#endif // EDITORPAGE_H
