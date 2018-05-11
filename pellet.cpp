#include "pellet.h"

int Pellet::nrStart = 0;
int Pellet::nrEaten = 0;

Pellet::Pellet(QGraphicsItem *parent) :
    GameObject(parent) // initialise base class member
{
    radius = 2; // within boundingRect
    eatenState = false;
    nrStart += 1;
}

Pellet::~Pellet()
{

}

QRectF Pellet::boundingRect() const
{
    return QRectF(-radius, -radius, 2*radius, 2*radius);
}

void Pellet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget); // prevent compiler from generating warnings regarding unused parameters
    Q_UNUSED(option);

    painter->setPen(Qt::NoPen); // no outer boundary line
    if (!eatenState){
        painter->setBrush(Qt::white);
    }else{
        painter->setBrush(Qt::black);
    }
    painter->setRenderHint(QPainter::Antialiasing); // smoother edges

    painter->drawEllipse(mapFromParent(this->pos()),radius,radius); // drawEllipse(const QPointF& center, qreal rx, qreal ry)
}

//bool Pellet::operator ==(const Pellet &pellet)
//{
//    if (pos() == pellet.pos() && radius == pellet.radius){
//        return true;
//    }else{
//        return false;
//    }
//}

bool Pellet::getEatenState() const
{
    return eatenState;
}

void Pellet::setEatenState(bool state)
{
    eatenState = state;
    if(state == true){
        nrEaten += 1;
    }
    else if(state == false && nrEaten > 0){ // avoid nrEaten going negative after GameWindow::Retry resets possibly uneaten pellets
        nrEaten -= 1; // invoked in resetPellets and resetPowerballs (shared static member for base/derived classes)
    }
}
int Pellet::getNrStart()
{
    return nrStart;
}
int Pellet::getNrEaten()
{
    return nrEaten;
}

int Pellet::getRadius() const
{
    return radius;
}

void Pellet::setRadius(int value)
{
    radius = value;
}

