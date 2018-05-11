#ifndef PELLET_H
#define PELLET_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include "gameobject.h"

class Pellet : public GameObject
{
public:
    Pellet(QGraphicsItem *parent = 0);
    ~Pellet();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    int getRadius() const;
    void setRadius(int value);

    bool getEatenState() const;
    void setEatenState(bool state);

    static int getNrStart();
    static int getNrEaten();

//    bool operator ==(const Pellet& pellet);

protected:
    double radius;
    bool eatenState; // if eaten, paint black; else normal colour
    // Static variables in base class shared by derived classes
    static int nrStart; // number of pellets+powerballs at start
    static int nrEaten; // number of eaten pellets+powerballs
};

#endif // PELLET_H
