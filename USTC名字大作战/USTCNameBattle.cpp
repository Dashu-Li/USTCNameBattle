#include "USTCNameBattle.h"
#include <QtWidgets/QApplication>

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
	gameWindow = new GameWindow(this);								// ������Ϸ����
	gameWindow->setWindowFlag(Qt::Window);							// ������Ϸ����Ϊ���㴰��
	gameWindow->show();												// ��ʾ��Ϸ����
	connect(gameWindow, SIGNAL(destroyed()), qApp, SLOT(quit()));	// ������Ϸ���ڵ�destroyed�źŵ�quit��, �Ա�����Ϸ���ڹر�ʱ�˳�����
	this->hide();													// ����������
}