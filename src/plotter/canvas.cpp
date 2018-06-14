#include "canvas.h"
#include "plotdata.h"

#include <QPainter>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void Canvas::Plot(QString file)
{
    _file = file;
    update();
}

void Canvas::SetPen(QColor penColor)
{
    _pen = QPen(penColor);
}

void Canvas::SetBrush(QColor brushColor)
{
    _brush = QBrush(brushColor);
}

void Canvas::paintEvent(QPaintEvent * /* event */)
{
    PrintPointsFromFile(_file);
}

QPoint Canvas::PointFromFrame(Frame &frame, QPoint canvasSize, double maxTime, int margin)
{
    int x = frame.Time() * canvasSize.x() / maxTime;
    int y = canvasSize.y() * ((1 - frame.Value()) / 2);

    y += (margin / 2);

    return QPoint(x, y);
}

void Canvas::PrintPointsFromFile(QString path)
{
    PlotData reader(path);

    int margin = 40;
    QRect canvas(1, 1, width() -2, height() -2);
    QPoint canvasSize(canvas.width(), canvas.height() -margin);

    QPainter painter(this);

    painter.setPen(_brush.color());
    painter.setBrush(_brush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(_pen);
    painter.drawLine(canvas.topLeft(), canvas.topRight());
    if (reader.HasFrames()){

        QPoint p0(0, 0);
        QPoint p1;
        Frame frame;

        reader.Reset();
        painter.save();
        while (reader.HasNextFrame()) {
            frame = reader.Next();
            p1 = PointFromFrame(frame, canvasSize, reader.Length(), margin);
            painter.drawLine(p0, p1);
            p0 = p1;
        }
        painter.restore();
    }
}
