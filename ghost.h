#ifndef GHOST_H
#define GHOST_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>

#include "gamecharacter.h"
#include "gameboard.h"

typedef enum {red = 0, orange, blue, pink} Colours;

class Ghost : public GameCharacter
{
public:
    Ghost(QGraphicsItem *parent = 0);
    ~Ghost();
    QRectF boundingRect() const override final;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;
    void advance() override final;
    void move(GameBoard* map, QPoint pacmanPos); // extra parameter pacmanPos for ghosts; 2 arguments, no virtual/override

    Colours getColour() const;
    void setColour(Colours col);

    void initialMove(); // initial left/right motion in ghost box
    static bool getInitialState();
    static void setInitialState(bool state);
    bool isEscapingBox() const;
    void setEscapingBox(bool state);
    void escapeBox();

    static bool getScaredState();
    static void setScaredState(bool state);
    static bool getBlueWhiteState();
    static void setBlueWhiteState(bool state);
    int getEscapingTimeState() const;
    void setEscapingTimeState(int state);

    void goStartPos() override final;
    void goStartDirection() override final;

    QPixmap ghostLeft1, ghostLeft2;
    QPixmap ghostRight1, ghostRight2;
    QPixmap ghostUp1, ghostUp2;
    QPixmap ghostDown1, ghostDown2;
    QPixmap ghostWhite1, ghostWhite2;
    QPixmap ghostBlue1, ghostBlue2;


private:
    Colours colour;

    static bool initialState; // initially ghosts stuck in ghost box; includes escaping period
    bool escapingBox; // true only when ghost is escaping rectangle: initial phase and after being eaten
    int escapingTimeState; // controls how long ghosts move up to escape

    static bool scaredState; // false = not scared, true = scared; static: shared state among all instances
    static bool blueWhiteState; // false = constant blue, true = blue/white oscillation
};

#endif // GHOST_H
