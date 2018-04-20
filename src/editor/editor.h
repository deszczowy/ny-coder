#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class ExtensionBar;

class Editor : public QPlainTextEdit
{
    Q_OBJECT
public:
    Editor(QWidget *parent = 0);

    void LineNumberAreaPaintEvent(QPaintEvent *event);
    int LineNumberAreaWidth();

    void LoadFile(QString path);
    QString GetPath();
    QString GetContent();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void UpdateLineNumberAreaWidth(int newBlockCount);
    void SelectCurrentLine();
    void UpdateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *_extensionBar;
    QString _path;
};

#endif // EDITOR_H
