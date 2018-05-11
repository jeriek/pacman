#ifndef POWERBALL_H
#define POWERBALL_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include "pellet.h"

class PowerBall : public Pellet
{
public:
    PowerBall(QGraphicsItem *parent = 0);
    ~PowerBall();
    QRectF boundingRect() const override final;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;

//    bool operator ==(const PowerBall& powerball);
};

#endif // POWERBALL_H
