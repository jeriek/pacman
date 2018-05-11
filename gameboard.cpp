#include "gameboard.h"
#include <QFile>
#include <QMessageBox>
#include "pellet.h"
#include "powerball.h"
#include "gamecharacter.h"


GameBoard::GameBoard(QGraphicsItem *parent) :
    GameObject(parent)
{
    backgroundMap.load(":/Images/PacManMazeResize.jpg");

    // Find accessible coordinates for Pacman (defined by map)
    setupPaths();
    // Define pellet/power ball positions
    setupPowerBallPositions(); // before setting up pellets: else both in corners
    setupPelletPositions();

    // Create pellet/power ball objects
    setPowerBalls();
    setPellets();
}

GameBoard::~GameBoard()
{

}

QRectF GameBoard::boundingRect() const
{
    return QRectF(0,0,435,480); // 435x480 maze -> square grid points every 15 px
}

void GameBoard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget); // prevent compiler from generating warnings regarding unused parameters
    Q_UNUSED(option);
    painter->drawPixmap(0,0,435,480,backgroundMap);

    // Below: not the proper way! Make pellets paint themselves
//    foreach (QPoint pelletPoint, pelletPointsVector){
//        painter->setPen(Qt::NoPen); // no outer boundary line
//        painter->setBrush(Qt::white);
//        painter->setRenderHint(QPainter::Antialiasing); // smoother edges

//        painter->drawEllipse(pelletPoint,2,2); // drawEllipse(const QPointF& center, qreal rx, qreal ry)
//    }
//    foreach (QPoint powerBallPoint, powerBallPointsVector){
//        painter->setPen(Qt::NoPen); // no outer boundary line
//        painter->setBrush(Qt::yellow);
//        painter->setRenderHint(QPainter::Antialiasing); // smoother edges

//        painter->drawEllipse(powerBallPoint,4,4); // drawEllipse(const QPointF& center, qreal rx, qreal ry)
//    }
}

QVector<QPoint> GameBoard::getPelletPoints() const
{
    return pelletPointsVector;
}

QVector<QPoint> GameBoard::getPowerBallPoints() const
{
    return powerBallPointsVector;
}

bool GameBoard::isAccessible(QPoint mapPoint) const
{
    return pathPointsVector.contains(mapPoint);
}

bool GameBoard::isAccessible(Directions dir, QPoint currentPos) const
{
    QPoint nextPoint = findNextPoint(dir,currentPos);
    return isAccessible(nextPoint);
}

QVector<Pellet *> GameBoard::getPelletsVector() const
{
    return pelletsVector;
}

QVector<PowerBall *> GameBoard::getPowerBallsVector() const
{
    return powerBallsVector;
}

void GameBoard::setPowerBallsVector(const QVector<PowerBall *> &value)
{
    powerBallsVector = value;
}

void GameBoard::setPelletsVector(const QVector<Pellet *> &value)
{
    pelletsVector = value;
}


void GameBoard::setupPaths()
{
    // Set up Pacman path nodes (see drawing)
    QPoint A(30,30);
    QPoint Q(105,30);
    QPoint B(195,30);
    QPoint C(240,30);
    QPoint R(330,30);
    QPoint D(405,30);

    QPoint E(30,90);
    QPoint GGG(150,90);
    QPoint O(195,90);
    QPoint P(240,90);
    QPoint HHH(285,90);
    QPoint F(405,90);

    QPoint G(30,135);
    QPoint H(105,135);
    QPoint I(150,135);
    QPoint J(195,135);
    QPoint K(240,135);
    QPoint L(285,135);
    QPoint M(330,135);
    QPoint N(405,135);

    QPoint S(150,180);
    QPoint T(195,180);
    QPoint U(240,180);
    QPoint V(285,180);

    QPoint W(0,225); // tunnel
    QPoint X(150,225);
    QPoint Y(285,225);
    QPoint Z(450,225); // tunnel

    QPoint AA(150,270);
    QPoint BB(285,270);

    QPoint CC(30,315);
    QPoint DD(150,315);
    QPoint EE(195,315);
    QPoint FF(240,315);
    QPoint GG(285,315);
    QPoint HH(405,315);

    QPoint II(30,360);
    QPoint JJ(60,360);
    QPoint KK(105,360);
    QPoint LL(150,360);
    QPoint MM(195,360);
    QPoint NN(240,360);
    QPoint OO(285,360);
    QPoint PP(330,360);
    QPoint QQ(375,360);
    QPoint RR(405,360);

    QPoint SS(30,405);
    QPoint TT(60,405);
    QPoint UU(105,405);
    QPoint VV(150,405);
    QPoint WW(195,405);
    QPoint XX(240,405);
    QPoint YY(285,405);
    QPoint ZZ(330,405);
    QPoint AAA(375,405);
    QPoint BBB(405,405);

    QPoint CCC(30,450);
    QPoint DDD(195,450);
    QPoint EEE(240,450);
    QPoint FFF(405,450);

    // 21 horizontal lines

    connectPoints(GGG,O);
    connectPoints(P,HHH);

    connectPoints(A,B);
    connectPoints(C,D);
    connectPoints(E,F);
    connectPoints(G,H);
    connectPoints(I,J);
    connectPoints(K,L);
    connectPoints(M,N);
    connectPoints(S,V);
    connectPoints(W,X);
    connectPoints(Y,Z);
    connectPoints(AA,BB);
    connectPoints(CC,EE);
    connectPoints(FF,HH);
    connectPoints(II,JJ);
    connectPoints(KK,PP);
    connectPoints(QQ,RR);
    connectPoints(SS,UU);
    connectPoints(VV,WW);
    connectPoints(XX,YY);
    connectPoints(ZZ,BBB);
    connectPoints(CCC,FFF);

    // 24 vertical lines
    connectPoints(A,G);
    connectPoints(CC,II);
    connectPoints(SS,CCC);
    connectPoints(JJ,TT);
    connectPoints(Q,UU);
    connectPoints(GGG,I);
    connectPoints(S,DD);
    connectPoints(LL,VV);
    connectPoints(B,O);
    connectPoints(J,T);
    connectPoints(EE,MM);
    connectPoints(WW,DDD);
    connectPoints(C,P);
    connectPoints(K,U);
    connectPoints(FF,NN);
    connectPoints(XX,EEE);
    connectPoints(HHH,L);
    connectPoints(V,GG);
    connectPoints(OO,YY);
    connectPoints(R,ZZ);
    connectPoints(QQ,AAA);
    connectPoints(D,N);
    connectPoints(HH,RR);
    connectPoints(BBB,FFF);
}

void GameBoard::connectPoints(QPoint pointA, QPoint pointB) // QPoint: integer precision
{
    QPoint newPathPoint;

    // Horizontal lines
    if (pointA.x() == pointB.x()){
        int yMin = std::min(pointA.y(),pointB.y());
        int yMax = std::max(pointA.y(),pointB.y());
        for (int y = yMin; y <= yMax; ++y){
            newPathPoint.setX(pointA.x());
            newPathPoint.setY(y);
            if (!pathPointsVector.contains(newPathPoint)){
                pathPointsVector.append(newPathPoint);
            }
        }
    }
    // Vertical lines
    else if (pointA.y() == pointB.y()){
        int xMin = std::min(pointA.x(),pointB.x());
        int xMax = std::max(pointA.x(),pointB.x());
        for (int x = xMin; x <= xMax; ++x){
            newPathPoint.setX(x);
            newPathPoint.setY(pointA.y());
            if (!pathPointsVector.contains(newPathPoint)){
                pathPointsVector.append(newPathPoint);
            }
        }
    }
}

void GameBoard::setupPelletPositions()
{

    // Every 15 pixels a new pellet, along Pacman paths
    QPoint newPelletPoint;
    for (int x=15; x<435; x+=15) // start at 10 so not for x=0 and 5 else trailing pellets on left tunnel edge
    {
        for (int y=0; y<480; y+=15){
            newPelletPoint.setX(x);
            newPelletPoint.setY(y);

            if (pathPointsVector.contains(newPelletPoint) &&
                !powerBallPointsVector.contains(newPelletPoint)){ // if reachable for Pacman and not a powerball
                   pelletPointsVector.append(newPelletPoint);
            }
        }
    }
}

void GameBoard::setupPowerBallPositions()
{
    QPoint powerBallPoint1(30,30);
    QPoint powerBallPoint2(405,30);
    QPoint powerBallPoint3(30,450);
    QPoint powerBallPoint4(405,450);

    powerBallPointsVector.append(powerBallPoint1);
    powerBallPointsVector.append(powerBallPoint2);
    powerBallPointsVector.append(powerBallPoint3);
    powerBallPointsVector.append(powerBallPoint4);

}

void GameBoard::setPellets()
{
    // Initialise Pellet objects for each point in pelletPositionsVector
    QPoint pelletPoint;
    foreach (QPoint pelletPoint, pelletPointsVector){
        // Create new Pellet object
        Pellet* newPellet = new Pellet(this); // GameBoard is parent and takes care of memory management: QObject inheritance
        newPellet->setPos(pelletPoint); // pelletPoint in parent coordinates
        pelletsVector.append(newPellet);
    }

}

void GameBoard::setPowerBalls()
{
    // Initialise PowerBall objects for each point in powerBallPointsVector
    QPoint powerBallPoint;
    foreach (QPoint powerBallPoint, powerBallPointsVector){
        // Create new Pellet object
        PowerBall* newPowerBall = new PowerBall(this); // GameBoard is parent and takes care of memory management: QObject inheritance
        newPowerBall->setPos(powerBallPoint); // pelletPoint in parent coordinates
        powerBallsVector.append(newPowerBall);
    }
}

void GameBoard::resetPellets()
{
    foreach(Pellet* pellet, pelletsVector){
        pellet->setEatenState(false);
    }
}

void GameBoard::resetPowerBalls()
{
    foreach(PowerBall* powerball, powerBallsVector){
        powerball->setEatenState(false);
    }
}

QPoint GameBoard::findNextPoint(Directions dir, QPoint currentPos)
{
    int posX = currentPos.x(); // current position
    int posY = currentPos.y();
    QPoint nextPoint(posX,posY);

    switch(dir)
    {
    case left:
        nextPoint.setX(posX-5);   // move 5 pixels
        break;
    case right:
        nextPoint.setX(posX+5);
        break;
    case up:
        nextPoint.setY(posY-5);
        break;
    case down:
        nextPoint.setY(posY+5);
        break;
    }
    return nextPoint;
}



