#include <QMessageBox>
#include <QKeyEvent>
#include <QTimer>

#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "ghost.h"
#include "pacman.h"
#include "gameboard.h"

#include <iostream>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    setWindowTitle("Pacman");

    lives = 3;
    level = 1;
    score = 0;
    highscore = 0;

    // Update rate in msec
    pacmanTimerInterval = 35;
    ghostTimerInterval = 90;

    pacmanBoard = new GameBoard; // adds item & all childen to scene, which takes ownership
    pacman = new Pacman;

    ghostsVector.reserve(4);
    ghostBlue = new Ghost;
    ghostBlue->setColour(blue);
    ghostsVector.append(ghostBlue);

    ghostOrange = new Ghost;
    ghostOrange->setColour(orange);
    ghostsVector.append(ghostOrange);

    ghostRed = new Ghost;
    ghostRed->setColour(red);
    ghostsVector.append(ghostRed);

    ghostPink = new Ghost;
    ghostPink->setColour(pink);
    ghostsVector.append(ghostPink);

    charactersVector.append(static_cast<GameCharacter*>(pacman));
    foreach (GameCharacter* ghost, ghostsVector) {
        charactersVector.append(ghost);
    }

    secondsTime = 0;
    gameTimer = new QTimer(this); // window handles ownership
    pacmanTimer = new QTimer(this);
    ghostTimer = new QTimer(this);

    isPlaying = false; // game does not start until space is pressed
    isPaused = false;
    isEnded = false;

    soundLoopState = 0;
    scaredTimeState = 0;
    initialTimeState = 0;

    screenText = new GameText; // scene will handle ownership
    screenText->setText("HIT SPACE TO START");

    scene = new QGraphicsScene(this); // window handles ownership of scene
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    scene->setSceneRect(0,0,435,480);
    ui->graphicsView->setSceneRect(scene->sceneRect());
    scene->addItem(pacmanBoard);
    scene->addItem(screenText);

    // Connect signals and slots (most work already done via Designer)
    connect(gameTimer,SIGNAL(timeout()), this,SLOT(lcdTimeUpdate()));
    connect(pacmanTimer,SIGNAL(timeout()), this,SLOT(updateGame()));
    connect(ghostTimer,SIGNAL(timeout()), this,SLOT(updateGhosts()));

    // Set keyboard focus on this widget
    setFocus();
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::checkGameState()
{
    // If all pellets/powerballs eaten: increment level, set highscore if needed, restart game
    if (Pellet::getNrEaten() == Pellet::getNrStart()){ // if all pellets+powerballs are eaten
        nextLevel();
    }

    // Check if Pacman collided with a ghost and whether ghost was scared
    foreach (Ghost* ghost, ghostsVector){ // loop ghostsVector for correct scoring if multiple simultaneous collisions
        if (pacman->collidesWithItem(ghost)){ // if colliding
            if (Ghost::getScaredState()){ // if scared: score increment, ghost back to startPos
                score += 100;
                ui->lcdScore->display(score); // immediately update score
                sound->play(":/Sounds/EatGhosts.wav");
                ghost->goStartPos();
                ghost->goStartDirection();
                ghost->setMoveState(false);
                ghost->setInitialState(false); // not true, else oscillates again
                ghost->setEscapingBox(true);
            }
            else { // if not scared: Pacman dies
                if (lives > 0){
                    lives -= 1;
                    ui->lcdLives->display(lives); // immediately update lives
                    sound->play(":/Sounds/PacmanDeath.wav");
                    if (score > highscore){ // if better score, already adjust highscore
                        highscore = score;
                        ui->lcdHighscore->display(score); // immediately update highscore
                    }
                    nextLife(); // continue/restart game
                }
                else { // game ends if no lives left
                    if (score > highscore){ // if better score, adjust highscore
                        highscore = score;
                        ui->lcdHighscore->display(score); // immediately update highscore
                    }
                    endGame();
                }
            }
        }
    }
}

void GameWindow::startGame()
{
    isPlaying = true;
    screenText->setText(""); // remove "HIT SPACE" line

    foreach (GameCharacter* gameCharacter, charactersVector){
        scene->addItem(gameCharacter);
    }
    scene->update(scene->sceneRect());

    gameTimer->start(1000); // emit signal every second = 1000 ms
    pacmanTimer->start(pacmanTimerInterval);
    ghostTimer->start(ghostTimerInterval-level*7); // ghosts become quicker throughout levels

    sound->play(":/Sounds/StartGame.wav");
}

void GameWindow::nextLife()
{
    // Reset states, no reloading of pellets/powerballs
    scaredTimeState = 0;
    soundLoopState = 0;
    initialTimeState = 0;

    foreach (GameCharacter* gameCharacter, charactersVector){ // polymorphic, for all characters
        gameCharacter->goStartPos();
        gameCharacter->goStartDirection();
        gameCharacter->setMoveState(false);
    }

    foreach (Ghost* ghost, ghostsVector){ // specifically for ghosts
        ghost->setScaredState(false);
        ghost->setBlueWhiteState(false);
        ghost->setInitialState(false);
        ghost->setEscapingBox(true);
    }

}

void GameWindow::nextLevel()
{
    if (score > highscore){ // if better score, adjust highscore
        highscore = score;
        ui->lcdHighscore->display(highscore); // immediately update lcdHighscore
    }

    ++level;
    if (level%3 == 0){ // extra life every 3 levels
        ++lives;
    }
    ui->lcdLevel->display(level);
    ui->lcdLives->display(lives);

    sound->play(":/Sounds/ExtraLife.wav"); // always play this sound even if no extra life

    pacmanBoard->resetPellets();
    pacmanBoard->resetPowerBalls();

    scaredTimeState = 0;
    soundLoopState = 0;
    initialTimeState = 0;

    foreach (GameCharacter* gameCharacter, charactersVector){ // polymorphic, for all characters
        gameCharacter->goStartPos();
        gameCharacter->goStartDirection();
        gameCharacter->setMoveState(false);
    }

    foreach (Ghost* ghost, ghostsVector){ // specifically for ghosts
        ghost->setScaredState(false);
        ghost->setBlueWhiteState(false);
        ghost->setInitialState(true);
        ghost->setEscapingBox(false);
    }

    pacmanTimer->start(pacmanTimerInterval);
    ghostTimer->start(ghostTimerInterval-level*7); // ghosts become quicker throughout levels
}

void GameWindow::endGame()
{
    isEnded = true;

    gameTimer->stop();
    pacmanTimer->stop();
    ghostTimer->stop();

    foreach (GameCharacter* gameCharacter, charactersVector){
        scene->removeItem(gameCharacter); // ownership passed on from scene to GameWindow
    }

    scene->removeItem(pacmanBoard);
    screenText->setText("GAME OVER");
    scene->update(scene->sceneRect());

    sound->play(":/Sounds/Intermission.wav");

    // Display "Game Over!" box
    QMessageBox messageBox;
    messageBox.warning(this,"Game Over","Game Over!");
}

void GameWindow::pauseGame()
{
    if (!isPlaying){ // if game did not start yet
        return;
    }
    if (!isPaused) { // not paused
        gameTimer->stop();
        pacmanTimer->stop();
        ghostTimer->stop();
        isPaused = true;
    }
    else{ // restart
        gameTimer->start();
        pacmanTimer->start();
        ghostTimer->start();
        isPaused = false;
    }
    update(); // update widget, schedule new repaint
    setFocus();
}

void GameWindow::updateGame()
{

    // Check game status (next level/end game/ate ghost/next life)
    checkGameState();

    // Display lives and level
    ui->lcdLevel->display(level);
    ui->lcdLives->display(lives);

    // Move Pacman according to set directions
    if (!Ghost::getInitialState()){ // once ghosts escaped from box
        pacman->move(pacmanBoard);
    }
    // Ghosts moved separately in updateGhosts()

    // Check if Pacman bumped into pellets/powerballs
    foreach (Pellet* pellet, pacmanBoard->getPelletsVector()){
        if (!pellet->getEatenState() && pacman->collidesWithItem(pellet)){ // if pellet not yet eaten and colliding: eat
            pellet->setEatenState(true);
            ++score; // +1 per pellet
            ui->lcdScore->display(score);

            // Alternate sounds when eating pellets
            if (soundLoopState == 0){ // play annoying sound only once every 7 iterations
                sound->play(":/Sounds/Chomping.wav");
            }
            else if (soundLoopState % 2 == 0 && soundLoopState < 7){ // 2,4,6: shorter sound
                sound->play(":/Sounds/Zip.wav");
            }
            ++soundLoopState;
            if (soundLoopState == 8){
                soundLoopState = 0; // reset every couple of iterations
            }
        }
    }

    foreach (PowerBall* powerball, pacmanBoard->getPowerBallsVector()){
        if (!powerball->getEatenState() && pacman->collidesWithItem(powerball)){ // if powerball not yet eaten and colliding: eat
            powerball->setEatenState(true);
            score += 20; // +20 per powerball
            ui->lcdScore->display(score);
            sound->play(":/Sounds/EatingCherry.wav");
            sound->play(":/Sounds/Siren.wav");

            // If ghosts already scared: restart scaredTimeState
            if (Ghost::getScaredState()){
                scaredTimeState = 0;
                Ghost::setBlueWhiteState(false); // ensure possible flashing stops
            }else{
                Ghost::setScaredState(true); // in any case, make ghosts scared
                // Immediately reverse ghost direction in transition to scared state
                foreach (Ghost* ghost, ghostsVector){
                    Directions oppositeDirection = static_cast<Directions>( (ghost->getDirection()+2)%4 );
                    ghost->setDirection(oppositeDirection);
                    ghost->setMoveState(true); // ensure ghost moves (maybe not necessary)
                }
            }
        }
    }

    // If ghosts are scared: let them move slower and increment scaredTimeState
    if (Ghost::getScaredState()){
        ++scaredTimeState;
        if (scaredTimeState == 1){ // first iteration in scared state
            ghostTimer->setInterval(ghostTimerInterval*2); // update ghosts less frequently: become slower
        }
        else if (scaredTimeState == 160-level*8){ // after ~150 iterations, ghosts start flashing blue/white briefly
            Ghost::setBlueWhiteState(true);
        }
        else if (scaredTimeState == 210-level*8){ // return to normal shortly after flashing
            Ghost::setScaredState(false);
            Ghost::setBlueWhiteState(false);
            scaredTimeState = 0; // reset
            ghostTimer->setInterval(ghostTimerInterval-level*7); // same as before flashing starts
        }
    }

    // Advance animations
    foreach (GameCharacter* gameCharacter, charactersVector) {
        gameCharacter->advance(); // polymorphic
    }

    // Schedule redraw of whole scene
    scene->update(scene->sceneRect());
}

void GameWindow::updateGhosts()
{
    int pacmanPosX = pacman->pos().x(); // convert to integer necessary since pos() is QPointF
    int pacmanPosY = pacman->pos().y();

    foreach(Ghost* ghost, ghostsVector){
        if (!Ghost::getInitialState() && !ghost->isEscapingBox()){ //if not in initial sequence or escaping: just move
            ghost->move(pacmanBoard,QPoint(pacmanPosX,pacmanPosY));
        }
        else{ // in INITIAL STATE or ESCAPING RECTANGLE AFTER GHOST DEATH
            if (!ghost->isEscapingBox()){ // in initial state and not escaping rectangle: oscillation time

                // Loop to escape initial state after some iterations
                if (ghost->pos().x() == 220){ // CHANGE THIS
                    ++initialTimeState; // (only increment when ghosts cross middle of box, else can escape beside door)
//                    std::cout<<"---initialtimestate: "<<initialTimeState<<std::endl;
                }
                if (initialTimeState == 16){ //=4*4 after ghosts crosses middle 4 times, release ghosts
                    foreach(Ghost* ghost, ghostsVector){
                        ghost->setEscapingBox(true);
                    }
        //            Ghost::setEscapingBox(true);
                    initialTimeState = 0; // reset
                }
                ghost->initialMove(); // left/right oscillation
            }
            else if (ghost->isEscapingBox()){
                QPoint newPoint;
                int posX = ghost->pos().x();
                if (posX >= 210 && posX <= 225){ // between edges of the opening/door of the box
                    ghost->escapeBox();
                }
                else{ // try to move into open region [210,225] of box and move up on next iteration
                    ghost->initialMove(); // left/right oscillation
                }
            }
        }
    }
}

void GameWindow::lcdTimeUpdate()
{
    ++secondsTime; // increment on every call by gameTimer signal per 1000 ms
    ui->lcdTime->display(secondsTime);
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()){
    case Qt::Key_Space: // space starts game if not yet started
        setFocus(); // also ensuring this widget has keyboard focus; since sometimes when clicking outside game, focus lost
        if(!isPlaying){
            startGame();
        }
        break;

    case Qt::Key_H:
        on_pushHelp_clicked();
        break;

    case Qt::Key_Escape:
        on_pushExit_clicked();
        break;
    }

    if (!isEnded){ // if game not yet over (not requiring !Ghost::getInitialState(): can set direction already when ghosts in box)
        switch (event->key()){
        case Qt::Key_Left:
            if(!pacman->getMoveState()){ // if not moving
                pacman->setDirection(left);
                if(!Ghost::getInitialState()){ // allow reconsideration of direction before ghosts get out of initial box
                    pacman->setMoveState(true);
                }
            }
            pacman->setNextDirection(left); // do this in any case
            break;

        case Qt::Key_Right:
            if(!pacman->getMoveState()){ // if not moving
                pacman->setDirection(right);
                if(!Ghost::getInitialState()){ // allow reconsidering direction via setDirection before ghosts escape: no moveState set until initialState over
                    pacman->setMoveState(true);
                }
            }
            pacman->setNextDirection(right);
            break;

        case Qt::Key_Up:
            if(!pacman->getMoveState()){ // if not moving
                pacman->setDirection(up);
                if(!Ghost::getInitialState()){ // allow reconsideration of direction before ghosts get out of initial box
                    pacman->setMoveState(true);
                }
            }
            pacman->setNextDirection(up);
            break;

        case Qt::Key_Down:
            if(!pacman->getMoveState()){ // if not moving
                pacman->setDirection(down);
                if(!Ghost::getInitialState()){ // allow reconsideration of direction before ghosts get out of initial box
                    pacman->setMoveState(true);
                }
            }
            pacman->setNextDirection(down);
            break;

        case Qt::Key_P:
            pauseGame();
            break;
        }
    }
}

void GameWindow::on_pushPause_clicked()
{
    if (!isEnded){ // if game is not yet over
        pauseGame();
    }
}

void GameWindow::on_pushHelp_clicked()
{
    if(!isPaused && !isEnded){
        pauseGame();
    }
    QMessageBox helpBox; // open message box
    helpBox.about(this,"Help","<p align='center'><font><h2>Controls</h2></font>"
        "Press SPACE to start the game.<br>"
        "Use the arrow keys to move Pacman around.<br>"
        "Press P to pause the game.<br>Press ESC to leave the game.</p>");
    if(!isEnded){
        pauseGame(); // unpause after closing
    }
    setFocus();
}

void GameWindow::on_pushExit_clicked()
{
    close();
}

void GameWindow::on_pushRetry_clicked()
{
    // If Retry clicked DURING game reaching a highscore: update
    if (score > highscore){ // if better score, adjust highscore
        highscore = score;
        ui->lcdHighscore->display(highscore); // immediately update lcdHighscore
    }

    lives = 3;
    level = 1;
    score = 0;
    ui->lcdScore->display(score);

    if (isEnded){ // if game ended, need to add all items back to scene
        screenText->setText(""); // remove "GAME OVER" text
        scene->addItem(pacmanBoard);
        foreach (GameCharacter* gameCharacter, charactersVector){
            scene->addItem(gameCharacter);
            isEnded = false;
        }
    }

    pacmanBoard->resetPellets();
    pacmanBoard->resetPowerBalls();
//    std::cout<<Pellet::getNrEaten()<<"/"<<Pellet::getNrStart()<<std::endl;

    scaredTimeState = 0;
    soundLoopState = 0;
    initialTimeState = 0;

    foreach (GameCharacter* gameCharacter, charactersVector){ // polymorphic, for all characters
        gameCharacter->goStartPos();
        gameCharacter->goStartDirection();
        gameCharacter->setMoveState(false);
    }

    foreach (Ghost* ghost, ghostsVector){ // specifically for ghosts
        ghost->setScaredState(false);
        ghost->setBlueWhiteState(false);
        ghost->setInitialState(true);
        ghost->setEscapingBox(false);
    }

    isPaused = false;
    isPlaying = true;

    scene->update(scene->sceneRect());

    secondsTime = 0; // reset game time
    ui->lcdTime->display(secondsTime);
    gameTimer->start(1000); // emit signal every second = 1000 ms
    pacmanTimer->start(pacmanTimerInterval);
    ghostTimer->start(ghostTimerInterval-level*7);

    sound->play(":/Sounds/StartGame.wav");

    setFocus();
}
