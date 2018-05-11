#include "gametext.h"

GameText::GameText(QGraphicsItem *parent) :
    GameObject(parent)
{
    width = 360;
    height = 60;
    QPoint middlePoint(218,225); // central, at ghostbox
    setPos(middlePoint); // sets center position of boundingrect
    setZValue(8); // text on top of everything
    text = QString(); // null string
}

GameText::~GameText()
{

}

QRectF GameText::boundingRect() const
{
    return QRectF(-width/2,-height/2,width,height);
}

void GameText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget); // prevent compiler from generating warnings regarding unused parameters
    Q_UNUSED(option);

    if (!text.isEmpty()){ // only spend time if text has characters (not null, nor "")
//        QFont font = QFont("ArcadeClassic", 18, 77); // enum QFont:Weight
        QFont font = QFont("Courier", 18, 77); // enum QFont:Weight
        QPen pen(Qt::red);
        painter->setPen(pen);
        painter->setFont(font);
        painter->drawText(boundingRect(), Qt::AlignCenter, text);
    }
}

QString GameText::getText() const
{
    return text;
}

void GameText::setText(const QString& newText)
{
    text = newText;
}


