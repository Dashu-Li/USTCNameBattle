#include "GameWindow.h"
#include "Game.h"
#include <QMessageBox>

GameWindow::GameWindow(QWidget *parent)
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
	QString battleText = ui.battleEdit->toPlainText();	// ��ȡbattleEdit���ı�
	if (battleText.isEmpty()) {							// ����ı�Ϊ��, �򵯳���ʾ��
		QMessageBox::warning(this, "����", "����������");
		return;
	}
    QStringList names = battleText.split("\n", Qt::SkipEmptyParts);		// �Ի��з��ָ��ı�
	if (names.size() < 2) {												// �����������С��2, �򵯳���ʾ��
		QMessageBox::warning(this, "����", "������������");
		return;
	}
}

void GameWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
	delete this;
}
