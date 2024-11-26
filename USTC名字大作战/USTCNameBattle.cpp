#include "USTCNameBattle.h"
#include <QtWidgets/QApplication>

USTCNameBattle::USTCNameBattle(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	gameWindow = new GameWindow();															// ������Ϸ����
	connect(ui.gameStartButton, &QPushButton::clicked, this, &USTCNameBattle::GameStart);	// ����GameStartButton��clicked�źŵ�GameStart��
	connect(gameWindow, &GameWindow::Back, this, &USTCNameBattle::Back);					// ������Ϸ���ڵ�Back�źŵ�Back��
}

USTCNameBattle::~USTCNameBattle()
{}

void USTCNameBattle::GameStart()
{
	gameWindow->show();												// ��ʾ��Ϸ����
	this->hide();													// ����������
}

void USTCNameBattle::Back()
{
	this->show();													// ��ʾ������
	gameWindow->hide();												// ������Ϸ����
}