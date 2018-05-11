#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QSound>
#include <QShortcut>
#include <QTimer>
#include <QGraphicsScene>
#include <QVector>
#include "gameboard.h"
#include "pacman.h"
#include "ghost.h"
#include "gametext.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

private:
    Ui::GameWindow *ui;

    GameBoard *pacmanBoard; // memory managed by scene
    Pacman *pacman;
    Ghost *ghostRed;
    Ghost *ghostOrange;
    Ghost *ghostBlue;
    Ghost *ghostPink;
    QVector<Ghost*> ghostsVector;
    QVector<GameCharacter*> charactersVector; // Pacman and ghosts

    int lives;
    int level;
    int score, highscore;
    QSound *sound;
    GameText *screenText;
    int secondsTime; // game time in seconds, for lcd
    int soundLoopState;
    int scaredTimeState; // controls how many iterations ghosts stay scared
    int initialTimeState; // controls when ghosts escape box

    bool isPlaying;
    bool isPaused;
    bool isEnded; // at end of game

    QGraphicsScene *scene;
    QTimer *gameTimer;
    QTimer *pacmanTimer;
    QTimer *ghostTimer;

    int pacmanTimerInterval;
    int ghostTimerInterval;

protected:

    void checkGameState(); // check if all items eaten or collisions occurred
    void startGame(); // (re)start game at possibly incremented level
    void nextLife(); // move on to next life when Pacman dies
    void nextLevel(); // move on to next level when all pellets are eaten
    void endGame();
    void pauseGame();

    void keyPressEvent(QKeyEvent *event);

private slots: // automatically set private for buttons
    void on_pushPause_clicked();
    void on_pushRetry_clicked();
    void on_pushHelp_clicked();
    void on_pushExit_clicked();

    void updateGame(); // updates/advances directions and game states, moves Pacman
    void updateGhosts(); // ghosts move on separate timer and have initial sequence
    void lcdTimeUpdate();
};

#endif // GAMEWINDOW_H
