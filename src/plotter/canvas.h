#ifndef RENDERAREA_H
#define RENDERAREA_H

#include "frame.h"

#include <QBrush>
#include <QPen>
#include <QPixmap>
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

    QPoint PointFromFrame(Frame &frame, QPoint canvasSize, double maxTime, int margin);
    void PrintPointsFromFile(QString path);
};

#endif // RENDERAREA_H
