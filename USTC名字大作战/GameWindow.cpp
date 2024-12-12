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
	ui.battleEdit->setReadOnly(true);											// 设置battleEdit只读
	ui.battleStartButton->setEnabled(false);									// 禁用开始对战按钮
	ui.PlayAgainButton->setEnabled(true);										// 启用再玩一局按钮
	game = new Game;															// 创建游戏对象
	for (int i = 0; i < teams.size(); i++) {
		QStringList names = teams[i].split("\n");								// 以换行符分割每组文本为名字
		for (int j = 0; j < names.size(); j++)
			game->addPlayer(names[j].toStdString(), i);							// 添加玩家
	}
	displayStatus();															// 显示状态
	// TODO: 输出对战结果
	delete game;																// 删除游戏对象
}

void GameWindow::on_PlayAgainButton_clicked()
{
	// 清空状态显示
	QLayoutItem* item;
	while ((item = ui.statusLayout->takeAt(0)) != nullptr) {
		delete item->widget();
		delete item;
	}
	ui.battleEdit->clear();											// 清空battleEdit
	ui.battleEdit->setReadOnly(false);								// 设置battleEdit可写
	ui.battleStartButton->setEnabled(true);							// 启用开始对战按钮
	ui.PlayAgainButton->setEnabled(false);							// 禁用再玩一局按钮
}

void GameWindow::displayStatus()
{
	for (int i = 0; i < game->getTeamCount(); i++) {
		for (auto player : game->getTeams()[i]) {
			QLabel* playerName = new QLabel(QString::fromStdString(player->getName()), this);
			QProgressBar* healthBar = new QProgressBar(this);
			healthBar->setRange(0, 100);
			healthBar->setValue(player->getHp());
			healthBar->setFormat("%v/%m");
			healthBar->setAlignment(Qt::AlignCenter);
			healthBar->setStyleSheet("QProgressBar::chunk { background-color: #7CFC00; }");

			// 连接玩家的 hpChanged 信号到 QProgressBar 的 setValue 槽，以便在玩家生命值改变时自动更新血条
			connect(player, &Player::hpChanged, healthBar, &QProgressBar::setValue);

			ui.statusLayout->addWidget(playerName);
			ui.statusLayout->addWidget(healthBar);
		}
		if (i < game->getTeamCount() - 1)
			ui.statusLayout->addItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 添加占位符以空出一定距离
	}
}

void GameWindow::closeEvent(QCloseEvent* event)
{
	event->accept();
	delete this;
}
