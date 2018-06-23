#ifndef RENDERAREA_H
#define RENDERAREA_H

#include "frame.h"
#include "plotdata.h"

#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QPushButton>
#include <QWidget>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(QWidget *parent = 0);
    void Plot(QString file);
    void SetPen(QColor penColor);
    void SetBrush(QColor brushColor);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPen _pen;
    QBrush _brush;
    QString _file;
    QPoint _canvasSize;

    QPainter *_painter;
    PlotData *_plotData;

    QPushButton *_closeButton;

    int _margin;

    QPoint PointFromFrame(Frame &frame, double maxTime);
    void PrintPointsFromFile();

    void PaintGrid();
    void ReadPlotData();
    void DropPlotData();

    void AddCloseButton();

private slots:
    void onHide();
};

#endif // RENDERAREA_H
