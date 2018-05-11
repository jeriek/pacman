#ifndef GAMECHARACTER_H
#define GAMECHARACTER_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>

#include "gameobject.h"
#include "gameboard.h"

enum Directions{left = 0, up, right, down}; // in C++: typedef not even needed; enum makes custom type

class GameCharacter : public GameObject // abstract base class
{
public:
    GameCharacter(QGraphicsItem *parent = 0);
    virtual ~GameCharacter();
    virtual void advance() = 0;

    void setDirection(Directions dir);
    void setAnimationState(int state);
    virtual void goStartPos();
    virtual void goStartDirection();

    Directions getDirection() const;
    int getAnimationState() const;

    Directions getNextDirection() const;
    void setNextDirection(const Directions &value);

    void move(GameBoard *map); // position change depending on current/next direction and accessibility in map

    bool getMoveState() const;
    void setMoveState(bool value);

protected:
    int width, height;
    Directions direction;
    Directions nextDirection;
    int animationState;
    bool moveState;
    QPoint startPos;
};

#endif // GAMECHARACTER_H
