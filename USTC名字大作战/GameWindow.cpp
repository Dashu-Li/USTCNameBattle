﻿#include "GameWindow.h"
#include <QMessageBox>

GameWindow::GameWindow(QWidget* parent)
	: QMainWindow(parent), game(nullptr)
{
	ui.setupUi(this);
	ui.PlayAgainButton->setEnabled(false);													// 初始禁用再玩一局按钮
	ui.battleEdit->setPlaceholderText("每行输入一个名字\n组队对战时用空行隔开组队");			// 设置battleEdit默认提示字
	connect(this, &QObject::destroyed, qApp, &QApplication::quit);							// 连接游戏窗口的destroyed信号到quit槽, 以便在游戏窗口关闭时退出程序
	connect(ui.backButton, &QPushButton::clicked, this, &GameWindow::Back);					// 连接返回按钮的clicked信号到Back信号
}

GameWindow::~GameWindow()
{}

void GameWindow::on_battleStartButton_clicked()
{
	QString battleText = ui.battleEdit->toPlainText();				// 获取battleEdit的文本
	if (battleText.isEmpty()) {										// 如果文本为空, 则弹出提示框
		QMessageBox::warning(this, "错误", "请输入名字");
		return;
	}
	QStringList names = battleText.split("\n", Qt::SkipEmptyParts);	// 以换行符分割文本
	if (names.size() < 2) {											// 如果名字数量小于2, 则弹出提示框
		QMessageBox::warning(this, "错误", "名字数量不足");
		return;
	}
	ui.battleEdit->setReadOnly(true);								// 设置battleEdit只读
	ui.battleStartButton->setEnabled(false);						// 禁用开始对战按钮
	ui.PlayAgainButton->setEnabled(true);							// 启用再玩一局按钮
	game = new Game;												// 创建游戏对象
	for (int i = 0; i < names.size(); i++) {
		game->addPlayer(names[i].toStdString());					// 添加玩家
	}
	// TODO: 输出对战结果

	delete game;													// 删除游戏对象
}

void GameWindow::on_PlayAgainButton_clicked()
{
	ui.battleEdit->clear();											// 清空battleEdit
	ui.battleEdit->setReadOnly(false);								// 设置battleEdit可写
	ui.battleStartButton->setEnabled(true);							// 启用开始对战按钮
	ui.PlayAgainButton->setEnabled(false);							// 禁用再玩一局按钮
}

void GameWindow::closeEvent(QCloseEvent* event)
{
	event->accept();
	delete this;
}
