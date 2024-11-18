#include "USTCNameBattle.h"
#include <QtWidgets/QApplication>

USTCNameBattle::USTCNameBattle(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	connect(ui.GameStartButton, SIGNAL(clicked()), this, SLOT(GameStart()));	// 连接GameStartButton的clicked信号到GameStart槽
}

USTCNameBattle::~USTCNameBattle()
{}

void USTCNameBattle::GameStart()
{
	gameWindow = new GameWindow(this);								// 创建游戏窗口
	gameWindow->setWindowFlag(Qt::Window);							// 设置游戏窗口为顶层窗口
	gameWindow->show();												// 显示游戏窗口
	connect(gameWindow, SIGNAL(destroyed()), qApp, SLOT(quit()));	// 连接游戏窗口的destroyed信号到quit槽, 以便在游戏窗口关闭时退出程序
	this->hide();													// 隐藏主窗口
}