#include "powerball.h"

PowerBall::PowerBall(QGraphicsItem *parent) :
    Pellet(parent)
{
    radius = 5;
}

PowerBall::~PowerBall()
{

}

QRectF PowerBall::boundingRect() const
{
    return QRectF(-radius, -radius, 2*radius, 2*radius); // width = height = diameter
}

void PowerBall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget); // prevent compiler from generating warnings regarding unused parameters
    Q_UNUSED(option);

    painter->setPen(Qt::NoPen); // no outer boundary line
    if (!eatenState){
        painter->setBrush(Qt::yellow);
    }else{
        painter->setBrush(Qt::black);
    }
    painter->setRenderHint(QPainter::Antialiasing); // smoother edges

    painter->drawEllipse(mapFromParent(this->pos()),radius,radius); // drawEllipse(const QPointF& center, qreal rx, qreal ry)
}

//bool PowerBall::operator ==(const PowerBall &powerball)
//{
//    if (pos() == powerball.pos() && radius == powerball.radius){
//        return true;
//    }else{
//        return false;
//    }
//}

