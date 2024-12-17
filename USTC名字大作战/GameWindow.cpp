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
	if (!game->isTeamBattle()) game->Regroup();									// 如果是个人战, 则重新分组
	connect(game, &Game::generateAction, this, &GameWindow::displayAction);		// 连接游戏的generateAction信号到displayAction槽
	displayStatus();															// 显示状态
	game->GenerateGame();														// 生成对局
	disconnect(game, &Game::generateAction, this, &GameWindow::displayAction);	// 断开连接
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

void GameWindow::displayAction(Action* action)
{
	switch (action->getActionType()) {
	case Action::Attack:
		// 若暴击则显示发动暴击，否则显示发动攻击
		// 若闪避则显示被闪避
		// 紧接着显示目标受到伤害值，用红色显示数字
		// 若目标死亡则显示被击倒了
		ui.battleEdit->append(QString::fromStdString(action->getInitiator()->getName()) + (action->getIsCritical() ? "发起<font color='blue'>暴击</font>，" : "发起攻击，") + QString::fromStdString(action->getTarget()->getName()) + (action->getIsMiss() ? "<font color='blue'>闪避</font>，" : "") + "受到" + "<font color='red'>" + QString::number(action->getValue()) + "</font>" + "点伤害");
		if (action->getTarget()->isDead())
			ui.battleEdit->append("    " + QString::fromStdString(action->getTarget()->getName()) + "被击倒了");
		break;
	case Action::Heal:
		// 显示发动治疗，紧接着显示目标恢复生命值，用绿色显示数字
		ui.battleEdit->append(QString::fromStdString(action->getInitiator()->getName()) + "治疗了" + QString::fromStdString(action->getTarget()->getName()) + "恢复了" + "<font color='green'>" + QString::number(action->getValue()) + "</font>" + "点生命值");
		break;
	}
}

void GameWindow::displayStatus()
{
	ui.battleEdit->clear();											// 清空battleEdit
	for (int i = 0; i < game->getTeamCount(); i++) {
		for (auto player : game->getTeams()[i]) {
			// battleEdit一行显示一个玩家名字 HP 攻 防 智（暴击） 敏（闪避） 抗（回复）
			ui.battleEdit->append(QString::fromStdString(player->getName()) + " HP: " + QString::number(player->getHp()) + " 攻: " + QString::number(player->getAtk()) + " 防: " + QString::number(player->getDef()) + " 智: " + QString::number(player->getCrit()) + " 敏: " + QString::number(player->getMiss()) + " 抗: " + QString::number(player->getHeal()));

			QLabel* playerName = new QLabel(QString::fromStdString(player->getName()), this);
			QProgressBar* healthBar = new QProgressBar(this);
			healthBar->setRange(0, player->getHpMax());
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
	// 显示分割线分隔后面对战信息
	ui.battleEdit->append("--------------------------------------------------");
}

void GameWindow::closeEvent(QCloseEvent* event)
{
	event->accept();
	delete this;
}
