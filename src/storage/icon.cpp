#include "icon.h"

#include <QFile>
#include <QPainter>
#include <QSvgRenderer>

Icon::Icon()
{
}

QIcon Icon::GetIcon(QString resource, QString color)
{

    // open svg resource load contents to qbytearray
    QFile file(resource);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();
    // load svg contents to xml document and edit contents
    QDomDocument doc;
    doc.setContent(baData);
    // recurivelly change color
    SetAttrRecur(doc.documentElement(), "fill", color);
    // create svg renderer with edited contents
    QSvgRenderer svgRenderer(doc.toByteArray());
    // create pixmap target (could be a QImage)
    QPixmap pix(svgRenderer.defaultSize());
    pix.fill(Qt::transparent);
    // create painter to act over pixmap
    QPainter pixPainter(&pix);
    // use renderer to render over painter which paints on pixmap
    svgRenderer.render(&pixPainter);
    QIcon myicon(pix);
    // Use icon ....
    return myicon;
}


void Icon::SetAttrRecur(QDomElement elem, QString strattr, QString strattrval)
{
    if (elem.attribute("id").compare("icon-body") == 0)
    {
        elem.setAttribute(strattr, strattrval);
    }
    // loop all children
    for (int i = 0; i < elem.childNodes().count(); i++)
    {
        if (!elem.childNodes().at(i).isElement())
        {
            continue;
        }
        SetAttrRecur(elem.childNodes().at(i).toElement(), strattr, strattrval);
    }
}
