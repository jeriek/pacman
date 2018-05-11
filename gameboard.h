#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QVector>
#include "gameobject.h"
#include "pellet.h"
#include "powerball.h"
//#include "gamecharacter.h" // for Directions enum

enum Directions; // forward-declaration to avoid circular dependence, #include "gamecharacter.h" in .cpp

class GameBoard : public GameObject
{
public:
    GameBoard(QGraphicsItem *parent = 0);
    ~GameBoard();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool isAccessible(QPoint mapPoint) const; // whether mapPoint can be passed by Pacman/ghosts
    bool isAccessible(Directions dir, QPoint currentPos) const; // whether Pacman/ghosts can go in direction dir

    QVector<QPoint> getPelletPoints() const;
    QVector<QPoint> getPowerBallPoints() const;

    QVector<Pellet*> getPelletsVector() const;
    void setPelletsVector(const QVector<Pellet*> &value);
    QVector<PowerBall*> getPowerBallsVector() const;
    void setPowerBallsVector(const QVector<PowerBall *> &value);

    void removePellet(Pellet* pellet); // remove pellet from pelletsVector
    void removePowerBall(PowerBall* powerball); // remove powerball from powerballsVector
    void resetPellets();
    void resetPowerBalls();

    static QPoint findNextPoint(Directions dir, QPoint currentPos); // calculate target point given direction and currentPos

private:
    QVector<QPoint> pathPointsVector; // Pacman path point positions vector
    QVector<QPoint> pelletPointsVector; // pellet positions vector
    QVector<QPoint> powerBallPointsVector; // powerball positions vector

    QVector<Pellet*> pelletsVector; // pellet pointers vector
    QVector<PowerBall*> powerBallsVector; // powerball pointers vector

    QPixmap backgroundMap; // Pacman maze

    void setupPaths(); // set up walkable paths for Pacman
    void connectPoints(QPoint pointA, QPoint pointB); // add path connecting A/B to pathPointsVector

    void setupPelletPositions();
    void setupPowerBallPositions();
    void setPellets();
    void setPowerBalls();    
};

#endif // GAMEBOARD_H
