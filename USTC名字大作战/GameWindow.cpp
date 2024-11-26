#include "GameWindow.h"

GameWindow::GameWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(this, &QObject::destroyed, qApp, &QApplication::quit);				// ������Ϸ���ڵ�destroyed�źŵ�quit��, �Ա�����Ϸ���ڹر�ʱ�˳�����
	connect(ui.backButton, &QPushButton::clicked, this, &GameWindow::Back);		// ���ӷ��ذ�ť��clicked�źŵ�Back�ź�
}

GameWindow::~GameWindow()
{}

void GameWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
	delete this;
}
