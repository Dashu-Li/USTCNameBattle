#include "USTCNameBattle.h"
#include "GameWindow.h"

USTCNameBattle::USTCNameBattle(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	connect(ui.GameStartButton, SIGNAL(clicked()), this, SLOT(GameStart()));	// ����GameStartButton��clicked�źŵ�GameStart��
}

USTCNameBattle::~USTCNameBattle()
{}

void USTCNameBattle::GameStart()
{
	GameWindow* gameWindow = new GameWindow(this);		// ������Ϸ����
	gameWindow->show();									// ��ʾ��Ϸ����
	this->hide();										// ����������
}