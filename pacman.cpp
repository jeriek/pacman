#include <QMatrix>
#include "pacman.h"

Pacman::Pacman(QGraphicsItem *parent) :
    GameCharacter(parent)
{
    width = 20;
    height = 20;

    setAnimationState(0);
    startPos.setX(240); // Pacman starts game at point NN
    startPos.setY(360);
    goStartPos();
    goStartDirection();

    // Load Pacman images
    pacmanClosed.load(":/Images/PacManClosed.png");

    pacmanRight1.load(":/Images/PacManOpen1.png");
    pacmanRight2.load(":/Images/PacManOpen2.png");

    QMatrix rotationMatrix;
    rotationMatrix.rotate(90);

    pacmanDown1 = pacmanRight1.transformed(rotationMatrix);
    pacmanDown2 = pacmanRight2.transformed(rotationMatrix);

    pacmanLeft1 = pacmanDown1.transformed(rotationMatrix);
    pacmanLeft2 = pacmanDown2.transformed(rotationMatrix);

    pacmanUp1 = pacmanLeft1.transformed(rotationMatrix);
    pacmanUp2 = pacmanLeft2.transformed(rotationMatrix);

    pacmanDeath1.load(":/Images/PacManOpen1.png");
    pacmanDeath2.load(":/Images/PacManOpen2.png");
    pacmanDeath3.load(":/Images/PacManOpen3.png");
    pacmanDeath4.load(":/Images/PacManOpen4.png");
    pacmanDeath5.load(":/Images/PacManOpen5.png");


}

Pacman::~Pacman()
{

}

QRectF Pacman::boundingRect() const
{
    return QRectF(-width/2,-height/2,width,height);
}

void Pacman::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget); // prevent compiler from generating warnings regarding unused parameters
    Q_UNUSED(option);

    // Painter paints in local coordinates (independent of pos()!)
    int topLeftX = - width/2;
    int topLeftY = - height/2;

    // Link right image to direction and animationState loop
    if (animationState < 2){ // animationState 0,1
        painter->drawPixmap(topLeftX,topLeftY,width,height,pacmanClosed);
    }
    else {
        switch (direction){
            case left:
                if(animationState < 4){ // animationState 2,3
                    painter->drawPixmap(topLeftX,topLeftY,width,height,pacmanLeft1);
                }else if(animationState < 6){ // animationState 4,5
                    painter->drawPixmap(topLeftX,topLeftY,width,height,pacmanLeft2);
                }
                break;
            case up:
                if(animationState < 4){ // animationState 2,3
                    painter->drawPixmap(topLeftX,topLeftY,width,height,pacmanUp1);
                }else if(animationState < 6){ // animationState 4,5
                    painter->drawPixmap(topLeftX,topLeftY,width,height,pacmanUp2);
                }
                break;
            case right:
                if(animationState<4){ // animationState 2,3
                    painter->drawPixmap(topLeftX,topLeftY,width,height,pacmanRight1);
                }else if(animationState<6){ // animationState 4,5
                    painter->drawPixmap(topLeftX,topLeftY,width,height,pacmanRight2);
                }
                break;
            case down:
                if(animationState<4){ // animationState 2,3
                    painter->drawPixmap(topLeftX,topLeftY,width,height,pacmanDown1);
                }else if(animationState<6){ // animationState 4,5
                    painter->drawPixmap(topLeftX,topLeftY,width,height,pacmanDown2);
                }
                break;
        }
    }
// Could add death sequence, but tricky for timing
}

void Pacman::advance()
{
        if (animationState > 3){ // should become: if > 5
            animationState = 0;
        }
        else{
            ++animationState;
        }
}

void Pacman::setDeathStatus(bool status)
{
    deathStatus = status;
}

bool Pacman::isDead() const
{
    return deathStatus;
}

void Pacman::goStartDirection()
{
    setDirection(right); // initially Pacman points right (C< shape)
    setNextDirection(right); // else unexpected turn at first crossing if no keystroke
}



