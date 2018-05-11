#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>

#include "gamecharacter.h"

class Pacman : public GameCharacter
{
public:
    Pacman(QGraphicsItem *parent = 0);
    ~Pacman();
    QRectF boundingRect() const override final;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;
    void advance() override final;

    void goStartDirection() override final;

    void setDeathStatus(bool status);
    bool isDead() const;

    QPixmap pacmanClosed;
    QPixmap pacmanLeft1, pacmanLeft2;
    QPixmap pacmanRight1, pacmanRight2;
    QPixmap pacmanUp1, pacmanUp2;
    QPixmap pacmanDown1, pacmanDown2;
    QPixmap pacmanDeath1, pacmanDeath2, pacmanDeath3, pacmanDeath4, pacmanDeath5;


private:
    bool deathStatus;
};

#endif // PACMAN_H
