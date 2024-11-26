#include "GameWindow.h"

GameWindow::GameWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(this, &QObject::destroyed, qApp, &QApplication::quit);				// 连接游戏窗口的destroyed信号到quit槽, 以便在游戏窗口关闭时退出程序
	connect(ui.backButton, &QPushButton::clicked, this, &GameWindow::Back);		// 连接返回按钮的clicked信号到Back信号
}

GameWindow::~GameWindow()
{}

void GameWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
	delete this;
}
