#ifndef GAMETEXT_H
#define GAMETEXT_H

#include <QGraphicsItem>
#include <QString>
#include <QFont>
#include "gameobject.h"

class GameText : public GameObject
{
public:
    GameText(QGraphicsItem *parent = 0);
    ~GameText();
    QRectF boundingRect() const override final;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;

    QString getText() const;
    void setText(const QString &newText);

private:
    int width, height;
    QString text;

};

#endif // GAMETEXT_H
