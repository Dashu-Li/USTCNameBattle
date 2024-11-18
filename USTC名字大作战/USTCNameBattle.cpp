#include "USTCNameBattle.h"
#include "GameWindow.h"

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
	GameWindow* gameWindow = new GameWindow(this);		// 创建游戏窗口
	gameWindow->show();									// 显示游戏窗口
	this->hide();										// 隐藏主窗口
}