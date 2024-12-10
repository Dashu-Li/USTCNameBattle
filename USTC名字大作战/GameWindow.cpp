#include "GameWindow.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QLabel>

GameWindow::GameWindow(QWidget* parent)
	: QMainWindow(parent), game(nullptr)
{
	ui.setupUi(this);
	ui.PlayAgainButton->setEnabled(false);													// 初始禁用再玩一局按钮
	ui.battleEdit->setPlaceholderText("每行输入一个名字\n组队对战时用空行隔开组队");			// 设置battleEdit默认提示字
	connect(this, &QObject::destroyed, qApp, &QApplication::quit);							// 连接游戏窗口的destroyed信号到quit槽, 以便在游戏窗口关闭时退出程序
	connect(ui.backButton, &QPushButton::clicked, this, &GameWindow::Back);					// 连接返回按钮的clicked信号到Back信号

	statusLayout = new QVBoxLayout(ui.statusBrowser);
	ui.statusBrowser->setLayout(statusLayout);
}

GameWindow::~GameWindow()
{}

void GameWindow::on_battleStartButton_clicked()
{
	QString battleText = ui.battleEdit->toPlainText().trimmed();				// 获取battleEdit的文本并去除首尾空行
	if (battleText.isEmpty()) {													// 如果文本为空, 则弹出提示框
		QMessageBox::warning(this, "错误", "请输入名字");
		return;
	}
	QStringList teams = battleText.split(QRegularExpression("\\n\\s*\\n"));		// 以连续空行分割文本为组
	if (teams.size() == 1) {													// 如果只有一个组, 则为个人战
		QStringList names = battleText.split("\n");								// 以换行符分割文本为名字
		if (names.size() < 2) {													// 如果名字数量小于2, 则弹出提示框
			QMessageBox::warning(this, "错误", "名字数量不足");
			return;
		}
	}
	ui.battleEdit->setReadOnly(true);										// 设置battleEdit只读
	ui.battleStartButton->setEnabled(false);								// 禁用开始对战按钮
	ui.PlayAgainButton->setEnabled(true);									// 启用再玩一局按钮
	game = new Game;														// 创建游戏对象
	for (int i = 0; i < teams.size(); i++) {
		QStringList names = teams[i].split("\n");							// 以换行符分割每组文本为名字
		for (int j = 0; j < names.size(); j++)
			game->addPlayer(names[j].toStdString(), i);						// 添加玩家
	}
	displayStatus();														// 显示状态
	// TODO: 输出对战结果
	delete game;															// 删除游戏对象
}

void GameWindow::on_PlayAgainButton_clicked()
{
	ui.battleEdit->clear();											// 清空battleEdit
	ui.statusBrowser->clear();										// 清空statusBrowser
	ui.battleEdit->setReadOnly(false);								// 设置battleEdit可写
	ui.battleStartButton->setEnabled(true);							// 启用开始对战按钮
	ui.PlayAgainButton->setEnabled(false);							// 禁用再玩一局按钮
}

/*
void GameWindow::displayStatus()
{
	// 使用HTML文本，一个玩家显示两行：第一行为该玩家名字，第二行为绿色血条；每个玩家用空行分隔
	QString status;
	for (int i = 0; i < game->getTeamCount(); ++i) {
		for (auto player : game->getTeams()[i]) {
			status += QString("<b>%1</b><br>").arg(QString::fromStdString(player->getName()));
			// 绘制绿色矩形条，长度为玩家生命值，生命值100对应最大长度为statusBrowser宽度，按比例缩放
			status += QString("<span style=\"background-color: green; width: %1px;\">&nbsp;</span><br>")
				.arg(player->getHp() * ui.statusBrowser->width() / 100);
		}
		status += "<br>";
	}
	ui.statusBrowser->setHtml(status);
}
*/

void GameWindow::displayStatus()
{
	// 清空之前的布局
	QLayoutItem* item;
	while ((item = statusLayout->takeAt(0)) != nullptr) {
		delete item->widget();
		delete item;
	}

	for (int i = 0; i < game->getTeamCount(); ++i) {
		for (auto player : game->getTeams()[i]) {
			QLabel* playerName = new QLabel(QString::fromStdString(player->getName()), this);
			QProgressBar* healthBar = new QProgressBar(this);
			healthBar->setRange(0, 100);
			healthBar->setValue(player->getHp());

			statusLayout->addWidget(playerName);
			statusLayout->addWidget(healthBar);
		}
		statusLayout->addSpacing(10); // 每个队伍之间增加一些间距
	}
}

void GameWindow::closeEvent(QCloseEvent* event)
{
	event->accept();
	delete this;
}
