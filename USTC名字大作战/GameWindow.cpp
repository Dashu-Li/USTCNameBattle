#include "GameWindow.h"

GameWindow::GameWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

GameWindow::~GameWindow()
{}

void GameWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
	delete this;
}