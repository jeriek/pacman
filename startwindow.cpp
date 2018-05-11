#include "startwindow.h"
#include "ui_startwindow.h"
#include "gamewindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    setWindowTitle("Pacman");
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_startButton_clicked()
{
    GameWindow *pacmanWindow = new GameWindow();
    pacmanWindow->show();
    // Disallow window resize, else keyboard focus loss when clicking outside gameboard
    pacmanWindow->setFixedSize(pacmanWindow->size());
    close(); // close StartWindow
}
