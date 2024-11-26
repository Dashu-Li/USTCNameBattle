#include "USTCNameBattle.h"
#include <QtWidgets/QApplication>

USTCNameBattle::USTCNameBattle(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	gameWindow = new GameWindow();															// 创建游戏窗口
	connect(ui.gameStartButton, &QPushButton::clicked, this, &USTCNameBattle::GameStart);	// 连接GameStartButton的clicked信号到GameStart槽
	connect(gameWindow, &GameWindow::Back, this, &USTCNameBattle::Back);					// 连接游戏窗口的Back信号到Back槽
}

USTCNameBattle::~USTCNameBattle()
{}

void USTCNameBattle::GameStart()
{
	gameWindow->show();												// 显示游戏窗口
	this->hide();													// 隐藏主窗口
}

void USTCNameBattle::Back()
{
	this->show();													// 显示主窗口
	gameWindow->hide();												// 隐藏游戏窗口
}