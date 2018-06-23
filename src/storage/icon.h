#ifndef ICON_H
#define ICON_H

#include <QDomDocument>
#include <QIcon>

class Icon
{
public:
    Icon();

    QIcon GetIcon(QString resource, QString color);
private:
    void SetAttrRecur(QDomElement elem, QString strattr, QString strattrval);
};

#endif // ICON_H
