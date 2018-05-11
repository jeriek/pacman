#include "gamecharacter.h"
#include <iostream>

GameCharacter::GameCharacter(QGraphicsItem *parent) :
    GameObject(parent)
{
    direction = static_cast<Directions>(qrand()%4); // overwritten anyway
    nextDirection = direction;
    moveState = false;
    width = 0;
    height = 0;
    startPos.setX(0);
    startPos.setY(0);
}

GameCharacter::~GameCharacter()
{

}

void GameCharacter::setDirection(Directions dir)
{
    direction = dir;
}

void GameCharacter::setAnimationState(int state)
{
    animationState = state;
}

void GameCharacter::goStartPos()
{
    setPos(startPos);
}

void GameCharacter::goStartDirection()
{
    setDirection(direction);
}

Directions GameCharacter::getDirection() const
{
    return direction;
}

int GameCharacter::getAnimationState() const
{
    return animationState;
}
Directions GameCharacter::getNextDirection() const
{
    return nextDirection;
}

void GameCharacter::setNextDirection(const Directions &value)
{
    nextDirection = value;
}

void GameCharacter::move(GameBoard *map)
{
    // Shorter method now, using GameBoard::findNextPoint
    int posX = pos().x(); // current position
    int posY = pos().y();
    QPoint currentPos(posX,posY); // not just pos(): no automatic QPointF->QPoint conversion

    if (nextDirection != direction){
        if(map->isAccessible(nextDirection,currentPos)){
            setDirection(nextDirection);
        }
        // else: carry on in same direction
    }

    // In any case, also if nextDirection = direction: move to next point and set moveState
    // moveState controls what happens when character gets stuck against wall

    if (getMoveState()){
        if(map->isAccessible(direction,currentPos)){
            QPoint nextPoint = GameBoard::findNextPoint(direction,currentPos);
            setPos(nextPoint);
//            setMoveState(true); // always the case already
        }
        else{
            setMoveState(false);
        }

        // Teleport tunnel at y = 450/2
        posX = pos().x(); // new calculated position
        posY = pos().y();
        if (posY == 225){
            QPoint oppositePoint;
            oppositePoint.setY(225);
            if (posX <= 0+5){ // left tunnel edge (+/- 5 accounts for Pacman size)
                oppositePoint.setX(435-5); // right tunnel edge
                setPos(oppositePoint);
            }
            else if (posX >= 435-5){ // right tunnel edge
                oppositePoint.setX(0+5); // left tunnel edge
                setPos(oppositePoint);
            }
        }
    }
}

bool GameCharacter::getMoveState() const
{
    return moveState;
}

void GameCharacter::setMoveState(bool value)
{
    moveState = value;
}



