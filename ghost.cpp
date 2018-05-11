#include "ghost.h"
#include <QTime>
#include <iostream>

// Initialise static members (else: unresolved LNK2001)
bool Ghost::scaredState = false;
bool Ghost::blueWhiteState = false;
bool Ghost::initialState = true;
//  bool Ghost::escapingBox = false;

Ghost::Ghost(QGraphicsItem *parent) :
    GameCharacter(parent)
{
    width = 24;
    height = 24;

    setAnimationState(0);
    goStartPos();
    goStartDirection();

    setScaredState(false);
    setBlueWhiteState(false);
    setInitialState(true);
    setEscapingBox(false);
    setEscapingTimeState(0);

    // Default colour red to prevent random value
    // But: should always call setColour to load pixmaps for left/right/up/down images!
    colour = red;

    // Scared ghosts always get the same white/blue images
    ghostWhite1.load(":/Images/ScaredGhostWhite1.png");
    ghostWhite2.load(":/Images/ScaredGhostWhite2.png");
    ghostBlue1.load(":/Images/ScaredGhostBlue1.png");
    ghostBlue2.load(":/Images/ScaredGhostBlue2.png");
}

Ghost::~Ghost()
{

}

QRectF Ghost::boundingRect() const
{
    return QRectF(-width/2,-height/2,width,height);
}

void Ghost::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget); // prevent compiler from generating warnings regarding unused parameters
    Q_UNUSED(option);

    int topLeftX = - width/2; // implicit conversion possible; -2.5->-2, 2.5->2, no problems
    int topLeftY = - height/2;

    // Link right image to scaredState, direction and animationState loop
    painter->setOpacity(0.8); // more ghost-like
    if(scaredState){ // when scared
        if(blueWhiteState){ // true = blue/white oscillation
            if(animationState == 0){
                painter->drawPixmap(topLeftX,topLeftY,width,height,ghostBlue1);
            }else{ // animationState == 1
                painter->drawPixmap(topLeftX,topLeftY,width,height,ghostWhite2);
            }
        }
        else{ // false = constant blue
            if(animationState == 0){
                painter->drawPixmap(topLeftX,topLeftY,width,height,ghostBlue1);
            }else{ // animationState == 1
                painter->drawPixmap(topLeftX,topLeftY,width,height,ghostBlue2);
            }
        }
    }
    else{ // when not scared: eyes join motion
        switch (direction){
            case left:
                if(animationState == 0){
                    painter->drawPixmap(topLeftX,topLeftY,width,height,ghostLeft1);
                }else if(animationState == 1){
                    painter->drawPixmap(topLeftX,topLeftY,width,height,ghostLeft2);
                }
                break;
            case up:
                if(animationState == 0){
                    painter->drawPixmap(topLeftX,topLeftY,width,height,ghostUp1);
                }else if(animationState == 1){
                    painter->drawPixmap(topLeftX,topLeftY,width,height,ghostUp2);
                }
                break;
            case right:
                if(animationState == 0){
                    painter->drawPixmap(topLeftX,topLeftY,width,height,ghostRight1);
                }else if(animationState == 1){
                    painter->drawPixmap(topLeftX,topLeftY,width,height,ghostRight2);
                }
                break;
            case down:
                if(animationState == 0){
                    painter->drawPixmap(topLeftX,topLeftY,width,height,ghostDown1);
                }else if(animationState == 1){
                    painter->drawPixmap(topLeftX,topLeftY,width,height,ghostDown2);
                }
                break;
        }
    }
}

void Ghost::advance()
{
        if (animationState >= 1){
            animationState = 0;
        }
        else{
            ++animationState;
        }
}

void Ghost::move(GameBoard* map, QPoint pacmanPos)
{
    if (!moveState){ // when stuck, give ghost random direction
        setDirection(static_cast<Directions>(qrand()%4)); // explicit cast required by Qt, static->no runtime check
        setNextDirection(direction); // else unexpected turns possible
        setMoveState(true); // if direction inaccessible, GameCharacter::move() makes moveState false and new random direction chosen in this loop
    }
    else{ // adapt direction to Pacman location
        // Current ghost position
        int posX = pos().x();
        int posY = pos().y();

        switch (direction){
            case left:
            case right:
                if(posY < pacmanPos.y()){ // Pacman is under
                    if(!scaredState){
                        setNextDirection(down);
                    }else{ // when scared: move away from Pacman
                        setNextDirection(up);
                    }
                }else if(posY > pacmanPos.y()){ // Pacman is above
                    if(!scaredState){
                        setNextDirection(up);
                    }else{
                        setNextDirection(down);
                    }
                }
                break;
            case up:
            case down:
                if(posX > pacmanPos.x()){ // Pacman is left
                    if(!scaredState){
                        setNextDirection(left);
                    }else{
                        setNextDirection(right);
                    }
                }else if(posX < pacmanPos.x()){ // Pacman is right
                    if(!scaredState){
                        setNextDirection(right);
                    }else{
                        setNextDirection(left);
                    }
                }
                break;
        }
    }
    GameCharacter::move(map); // call base class function
}

void Ghost::initialMove()
{
    if (pos().x() <= 185){ // left ghostbox edge
        setDirection(right);
    }
    else if (pos().x() >= 250){ // right ghostbox edge
        setDirection(left);
    }

    int posX = pos().x();
    int posY = pos().y();
    QPoint newPoint(posX,posY);
    if (direction == left){
        newPoint.setX(posX-5);
    }
    else if (direction == right){
        newPoint.setX(posX+5);
    }
    setPos(newPoint);
}

Colours Ghost::getColour() const
{
    return colour;
}

void Ghost::setColour(Colours col)
{
    colour = col;

    // Load ghost images for all colours
    switch (colour){
        case red:
            ghostLeft1.load(":/Images/BlinkyLeft1.png");
            ghostLeft2.load(":/Images/BlinkyLeft2.png");
            ghostRight1.load(":/Images/BlinkyRight1.png");
            ghostRight2.load(":/Images/BlinkyRight2.png");
            ghostUp1.load(":/Images/BlinkyUp1.png");
            ghostUp2.load(":/Images/BlinkyUp2.png");
            ghostDown1.load(":/Images/BlinkyDown1.png");
            ghostDown2.load(":/Images/BlinkyDown2.png");
            break;
        case orange:
            ghostLeft1.load(":/Images/ClydeLeft1.png");
            ghostLeft2.load(":/Images/ClydeLeft2.png");
            ghostRight1.load(":/Images/ClydeRight1.png");
            ghostRight2.load(":/Images/ClydeRight2.png");
            ghostUp1.load(":/Images/ClydeUp1.png");
            ghostUp2.load(":/Images/ClydeUp2.png");
            ghostDown1.load(":/Images/ClydeDown1.png");
            ghostDown2.load(":/Images/ClydeDown2.png");
            break;
        case blue:
            ghostLeft1.load(":/Images/InkyLeft1.png");
            ghostLeft2.load(":/Images/InkyLeft2.png");
            ghostRight1.load(":/Images/InkyRight1.png");
            ghostRight2.load(":/Images/InkyRight2.png");
            ghostUp1.load(":/Images/InkyUp1.png");
            ghostUp2.load(":/Images/InkyUp2.png");
            ghostDown1.load(":/Images/InkyDown1.png");
            ghostDown2.load(":/Images/InkyDown2.png");
            break;
        case pink:
            ghostLeft1.load(":/Images/PinkyLeft1.png");
            ghostLeft2.load(":/Images/PinkyLeft2.png");
            ghostRight1.load(":/Images/PinkyRight1.png");
            ghostRight2.load(":/Images/PinkyRight2.png");
            ghostUp1.load(":/Images/PinkyUp1.png");
            ghostUp2.load(":/Images/PinkyUp2.png");
            ghostDown1.load(":/Images/PinkyDown1.png");
            ghostDown2.load(":/Images/PinkyDown2.png");
            break;
    }
}

bool Ghost::getScaredState()
{
    return scaredState;
}

void Ghost::setScaredState(bool state)
{
    scaredState = state;
}

bool Ghost::getBlueWhiteState()
{
    return blueWhiteState;
}

void Ghost::setBlueWhiteState(bool state)
{
    blueWhiteState = state;
}

void Ghost::goStartPos()
{
    static int seedAddition = 0; // otherwise same seed when qrand() in same msec (occurs in nextLife())
    QTime systemTime = QTime::currentTime();
//    std::cout<<"seed: "<<systemTime.msec()+seedAddition<<std::endl;
    qsrand(systemTime.msec()+seedAddition); // seed random generator with msec part of system time, else same when program rerun
    ++seedAddition;

    int startX = 185 + (qrand()%14)*5; // random in [185,250] box // (qrand()%2)? 215-(qrand()%2)*5 : 220+(qrand()%2)*5; // start left or right of middle, randomly
    startPos.setX(startX); // ghosts start in middle of ghost box
    startPos.setY(225);
    GameCharacter::goStartPos();
}

bool Ghost::getInitialState()
{
    return initialState;
}

void Ghost::setInitialState(bool state)
{
    initialState = state;
}

bool Ghost::isEscapingBox() const
{
    return escapingBox;
}

void Ghost::setEscapingBox(bool state)
{
    escapingBox = state;
}

void Ghost::escapeBox()
{
    int posX = pos().x();
    int posY = pos().y();
    QPoint newPoint(posX,posY-5);// move up, out of box; full 45 up too drastic but else inaccessible points
    setPos(newPoint);
    ++escapingTimeState;
    if (newPoint.y() == 180){ // jump of 45 to Pacman paths, enter normal motion loop
        Ghost::setInitialState(false); // static: for all ghosts! initial state comprises escaping period
        setEscapingBox(false); // enter normal loop
        setMoveState(true);
        setEscapingTimeState(0); // reset
    }
}

int Ghost::getEscapingTimeState() const
{
    return escapingTimeState;
}

void Ghost::setEscapingTimeState(int state)
{
    escapingTimeState = state;
}

void Ghost::goStartDirection()
{
    // Make ghosts alternate initial direction
    static int leftRight = 0;
    if (leftRight % 2 == 0){
        setDirection(left); // initially ghost goes to left
    }else{
        setDirection(right); // initially ghost goes to right
    }
    ++leftRight;
    setNextDirection(getDirection());
}







