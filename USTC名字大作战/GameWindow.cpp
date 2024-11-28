#include "GameWindow.h"
#include "Game.h"
#include <QMessageBox>

GameWindow::GameWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.battleEdit->setPlaceholderText("每行输入一个名字\n组队对战时用空行隔开组队");			// 设置battleEdit默认提示字
	connect(this, &QObject::destroyed, qApp, &QApplication::quit);							// 连接游戏窗口的destroyed信号到quit槽, 以便在游戏窗口关闭时退出程序
	connect(ui.backButton, &QPushButton::clicked, this, &GameWindow::Back);					// 连接返回按钮的clicked信号到Back信号
	connect(ui.battleStartButton, &QPushButton::clicked, this, &GameWindow::StartBattle);	// 连接开始对战按钮的clicked信号到startBattle槽
}

GameWindow::~GameWindow()
{}

void GameWindow::StartBattle()
{
	QString battleText = ui.battleEdit->toPlainText();	// 获取battleEdit的文本
	if (battleText.isEmpty()) {							// 如果文本为空, 则弹出提示框
		QMessageBox::warning(this, "错误", "请输入名字");
		return;
	}
	QStringList names = battleText.split("\n", Qt::SkipEmptyParts);		// 以换行符分割文本
	if (names.size() < 2) {												// 如果名字数量小于2, 则弹出提示框
		QMessageBox::warning(this, "错误", "名字数量不足");
		return;
	}
}

void GameWindow::closeEvent(QCloseEvent* event)
{
	event->accept();
	delete this;
}
