#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QCompleter>
#include <QUuid>
#include <QTabWidget>

#include <src/prompter/nyprompter.h>

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


    QString Content();
    QString Path();

    bool Save();
    bool SaveAs();
    bool IsNew();

    void SetContext(int index, QTabWidget *widget);
    void SetPrompter(NyPrompter *prompter);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;

private slots:
    void UpdateLineNumberAreaWidth(int);
    void SelectCurrentLine();
    void UpdateLineNumberArea(const QRect &rect, int dy);

    void SetModificationIndicator(bool modified);
    void IncorporateLineIntoList();
    void onPrompt(QString prompted);

private:
    QWidget *_extensionBar;
    NyPrompter *_prompter;

    QString _path;
    QString _fileName;
    bool _untitled;
    int _index;
    QTabWidget *_pages;
    bool _loaded;

    QString _indent;
    void CalculateIndent();

    QString TextUnderCursor() const;

    void LoadFile(QString path);

    QColor Color(QString themeField);

    QRect PromptGeometry(QString word);
};

#endif // EDITOR_H
