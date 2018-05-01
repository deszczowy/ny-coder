#ifndef EXTENSIONBAR_H
#define EXTENSIONBAR_H

#include <QWidget>

class Editor;

class ExtensionBar : public QWidget
{
public:
    ExtensionBar(Editor *editor);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Editor *connectedEditor;
};

#endif // EXTENSIONBAR_H