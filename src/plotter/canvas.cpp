/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#include "canvas.h"
#include "nyplotdata.h"

#include <QPainter>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    AddCloseButton();
}

void Canvas::Plot(QString file, double duration)
{
    _file = file;
    _duration = duration;
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

void Canvas::paintEvent(QPaintEvent *)
{
    _painter = new QPainter(this);
    ReadPlotData();
    PrintPointsFromFile();
    DropPlotData();
    delete _painter;
}

QPoint Canvas::PointFromFrame(NySoundFrame &frame, double maxTime)
{
    int x = frame.Time() * _canvasSize.x() / maxTime;
    int y = _canvasSize.y() * ((1 - frame.Value()) / 2);

    y += (_margin / 2);

    return QPoint(x, y);
}

void Canvas::PrintPointsFromFile()
{
    _margin = 40;
    QRect canvas(1, 1, width() -2, height() -2);
    _canvasSize = QPoint(canvas.width(), canvas.height() -_margin);

    _painter->setPen(_brush.color());
    _painter->setBrush(_brush);
    _painter->drawRect(QRect(0, 0, width() - 1, height() - 1));
    _painter->setRenderHint(QPainter::Antialiasing, true);

    _painter->setPen(_pen);
    _painter->save();
    if (_plotData->HasFrames()){

        QPoint p0(0, _canvasSize.y() / 2);
        QPoint p1;
        NySoundFrame frame;

        _plotData->Reset();

        while (_plotData->HasNextFrame()) {
            frame = _plotData->Next();
            p1 = PointFromFrame(frame, _plotData->Length());
            _painter->drawLine(p0, p1);
            p0 = p1;
        }

    }

    PaintGrid();
    _painter->drawLine(canvas.topLeft(), canvas.topRight());
    _painter->restore();
}

void Canvas::PaintGrid()
{
    QPoint topLeft(1, 0);
    QPoint topRight(_canvasSize.x(), 0);
    QPoint bottomLeft(1, _canvasSize.y() + _margin -1);
    QPoint bottomRight(_canvasSize.x(), _canvasSize.y() + _margin -1);

    QPoint zeroLeft(1, (_canvasSize.y() + _margin) / 2);
    QPoint zeroRight(_canvasSize.x(), (_canvasSize.y() + _margin) / 2);

    //_painter->setPen(QPen(QBrush(_pen.color()), 1, Qt::DotLine));
    //_painter->setBrush(_brush);

    //_painter->drawLine(topLeft, bottomLeft);
    _painter->drawLine(zeroLeft, zeroRight);
    //_painter->drawLine(topLeft, topRight);
    //_painter->drawLine(bottomLeft, bottomRight);

    QPen pen = _painter->pen();
    pen.setStyle(Qt::DotLine);
    _painter->setPen(pen);

    double step = _duration / 5;
    double x = step;
    QList<NySoundFrame> list;

    for (int i = 0; i < 4; ++i){
        list.append(NySoundFrame(x, -1));
        list.append(NySoundFrame(x, 1));
        x += step;
    }

    int i = 0;
    QPoint p0;
    QPoint p1;
    foreach (NySoundFrame frame, list) {
        if (i % 2 == 0){
            p0 = PointFromFrame(frame, _plotData->Length());
        } else {
            p1 = PointFromFrame(frame, _plotData->Length());
            _painter->drawLine(p0, p1);
            _painter->drawText(p0, QString::number(frame.Time()));
        }
        ++i;
    }
    _painter->setPen(_pen);
}

void Canvas::ReadPlotData()
{
    _plotData = new NyPlotData(_file);
}

void Canvas::DropPlotData()
{
    if (_plotData) delete _plotData;
}

void Canvas::AddCloseButton()
{
    if (!_closeButton){
        int w = 25;
        int x = 5;

        _closeButton = new QPushButton("x", this);

        _closeButton->setGeometry(x, x, w, w);
        _closeButton->setMinimumSize(w, w);
        _closeButton->show();

        connect(_closeButton, SIGNAL(clicked(bool)), this, SLOT(onHide()));
    }
}

void Canvas::onHide()
{
    hide();
}
