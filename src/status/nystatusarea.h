#ifndef NYSTATUSAREA_H
#define NYSTATUSAREA_H

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class NyStatusArea : public QWidget
{
    Q_OBJECT
public:
    NyStatusArea();

signals:

public slots:

private:
    void CreateLayout();
    void CreateLeftPanel();
    void CreateRightPanel();
    void CreateCenterPanel();
    void ArrangeLayout();

private:
    QLabel *_leftPanel;
    QLabel *_rightPanel;
    QLabel *_centerPanel;

    QHBoxLayout *_layout;
};

#endif // NYSTATUSAREA_H
