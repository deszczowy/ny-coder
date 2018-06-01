#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QCompleter>
#include <QUuid>

class QPaintEvent;
class QResizeEvent;
class QWidget;

class Editor : public QPlainTextEdit
{
    Q_OBJECT
public:
    Editor(QWidget *parent = 0, QString filePath = "");
    ~Editor();

    void LineNumberAreaPaintEvent(QPaintEvent *event);
    int LineNumberAreaWidth();

    void SetCompleter(QCompleter *completer);
    QCompleter *GetCompleter() const;


    QString Content();
    QString Path();

    bool Save();
    bool SaveAs();
    bool IsNew();



protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;

private slots:
    void UpdateLineNumberAreaWidth(int newBlockCount);
    void SelectCurrentLine();
    void UpdateLineNumberArea(const QRect &rect, int dy);
    void InsertCompletion(const QString &completion);

private:
    QWidget *_extensionBar;
    QCompleter *_completer;
    QAbstractItemModel *completionModel;
    QCompleter *completer;

    QString _path;
    bool _untitled;

    QString TextUnderCursor() const;
    QAbstractItemModel *modelFromFile(const QString & fileName);

    void LoadFile(QString path);
};

#endif // EDITOR_H
