#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>

class GameObject : public QGraphicsItem // abstract base class (not really necessary now, maybe later)
{
public:
    GameObject(QGraphicsItem *parent = 0);
    virtual ~GameObject();

    virtual QRectF boundingRect() const = 0; // pure virtual function
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;

};

#endif // GAMEOBJECT_H
