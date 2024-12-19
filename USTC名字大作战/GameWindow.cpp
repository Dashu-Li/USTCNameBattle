#include "GameWindow.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QLabel>
#include <QThread>
#include <QTableWidget>
#include <QHeaderView>

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
	QString battleText = ui.battleEdit->toPlainText().trimmed();										// 获取battleEdit的文本并去除首尾空行
	if (battleText.isEmpty()) {																			// 如果文本为空, 则弹出提示框
		QMessageBox::warning(this, "错误", "请输入名字");
		return;
	}
	QStringList teams = battleText.split(QRegularExpression("\\n\\s*\\n"));								// 以连续空行分割文本为组
	if (teams.size() == 1) {																			// 如果只有一个组, 则为个人战
		QStringList names = battleText.split("\n");														// 以换行符分割文本为名字
		if (names.size() < 2) {																			// 如果名字数量小于2, 则弹出提示框
			QMessageBox::warning(this, "错误", "名字数量不足");
			return;
		}
	}
	ui.battleEdit->setReadOnly(true);																	// 设置battleEdit只读
	ui.battleStartButton->setEnabled(false);															// 禁用开始对战按钮
	// QThread* thread = new QThread;																		// 创建线程
	game = new Game;																					// 创建游戏对象
	// game->moveToThread(thread);																			// 将游戏对象移动到线程
	connect(game, &Game::generateAction, this, &GameWindow::displayAction);								// 连接游戏的generateAction信号到displayAction槽，连接模式为直接连接
	connect(game, &Game::gameEnd, this, &GameWindow::GameEnd);											// 连接游戏的gameEnd信号到GameOver槽
	// thread->start();																					// 启动线程
	for (int i = 0; i < teams.size(); i++) {
		QStringList names = teams[i].split("\n");														// 以换行符分割每组文本为名字
		for (int j = 0; j < names.size(); j++)
			game->addPlayer(names[j].toStdString(), i);													// 添加玩家
	}
	if (!game->isTeamBattle()) game->Regroup();															// 如果是个人战, 则重新分组
	displayStatus();																					// 显示状态
	game->GenerateGame();																				// 生成对局
	disconnect(game, &Game::generateAction, this, &GameWindow::displayAction);							// 断开连接
	delete game;																						// 删除游戏对象
	// thread->quit();																						// 退出线程
	ui.PlayAgainButton->setEnabled(true);																// 启用再玩一局按钮
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
	sleepTime = 600;												// 设置间隔时间为600ms
}

void GameWindow::displayAction(Action* action)
{
	switch (action->getActionType()) {
	case Action::Attack:
		// 若暴击则显示发动暴击，否则显示发动攻击
		// 若闪避则显示被闪避
		// 紧接着显示目标受到伤害值，用红色显示数字
		// 若目标死亡则显示被击倒了
		ui.battleEdit->append(QString::fromStdString(action->getInitiator()->getName()) + (action->getIsCritical() ? " 发起<font color='blueviolet'>暴击</font>，" : " 发起攻击，") + QString::fromStdString(action->getTarget()->getName()) + (action->getIsMiss() ? "<font color='blueviolet'>闪避</font>，" : " ") + "受到" + "<font color='red'>" + QString::number(action->getDamage()) + "</font>" + "点伤害");
		if (action->getTarget()->isDead())
			ui.battleEdit->append("    " + QString::fromStdString(action->getTarget()->getName()) + " 被击倒了");
		break;
	case Action::Heal:
		// 显示发动治疗，紧接着显示目标恢复生命值，用绿色显示数字
		ui.battleEdit->append(QString::fromStdString(action->getInitiator()->getName()) + " 治疗 " + QString::fromStdString(action->getTarget()->getName()) + " 恢复了" + "<font color='green'>" + QString::number(action->getHeal()) + "</font>" + "点生命值");
		break;
	case Action::Lifesteal:
		// 显示发动吸血，紧接着显示目标受到伤害值，用红色显示数字
		// 若目标死亡则显示被击倒了
		// 紧接着显示发动者恢复生命值，用绿色显示数字
		ui.battleEdit->append(QString::fromStdString(action->getInitiator()->getName()) + " 发动<font color='blueviolet'>吸血</font>，" + QString::fromStdString(action->getTarget()->getName()) + (action->getIsMiss() ? " <font color='blueviolet'>闪避</font>，" : " ") + "受到" + "<font color='red'>" + QString::number(action->getDamage()) + "</font>" + "点伤害");
		if (action->getTarget()->isDead())
			ui.battleEdit->append("    " + QString::fromStdString(action->getTarget()->getName()) + " 被击倒了");
		ui.battleEdit->append("&nbsp;&nbsp;&nbsp;&nbsp;" + QString::fromStdString(action->getInitiator()->getName()) + " 恢复了" + "<font color='green'>" + QString::number(action->getHeal()) + "</font>" + "点生命值");
		break;
	case Action::Ascension:
		// 显示发动飞升，紧接着显示发动者攻击力提升
		ui.battleEdit->append(QString::fromStdString(action->getInitiator()->getName()) + " 发动<font color='blueviolet'>飞升</font>");
		break;
	case Action::Fire:
		// 显示发动点燃，紧接着显示目标受到伤害值，用红色显示数字
		// 若目标死亡则显示被击倒了
		ui.battleEdit->append(QString::fromStdString(action->getInitiator()->getName()) + " <font color='red'>点燃</font>了 " + QString::fromStdString(action->getTarget()->getName()) + (action->getIsMiss() ? "，" + QString::fromStdString(action->getTarget()->getName()) + " <font color='blueviolet'>闪避</font>" : ""));
		break;
	case Action::Freeze:
		// 显示发动冰冻，紧接着显示目标被冰冻
		ui.battleEdit->append(QString::fromStdString(action->getInitiator()->getName()) + " <font color='blue'>冰冻</font>了 " + QString::fromStdString(action->getTarget()->getName()) + (action->getIsMiss() ? "，" + QString::fromStdString(action->getTarget()->getName()) + " <font color='blueviolet'>闪避</font>" : ""));
		break;
	case Action::Burn:
		// 显示发动燃烧，紧接着显示目标受到伤害值，用红色显示数字
		// 若目标死亡则显示被击倒了
		ui.battleEdit->append(QString::fromStdString(action->getInitiator()->getName()) + " <font color='red'>燃烧</font>受到" + "<font color='red'>" + QString::number(action->getDamage()) + "</font>" + "点伤害");
		if (action->getInitiator()->isDead())
			ui.battleEdit->append("    " + QString::fromStdString(action->getInitiator()->getName()) + " 被击倒了");
		break;
	case Action::Extinguish:
		// 显示发动熄灭，紧接着显示目标熄灭燃烧
		ui.battleEdit->append(QString::fromStdString(action->getInitiator()->getName()) + " 的燃烧<font color='blue'>熄灭</font>了");
		break;
	case Action::Unfreeze:
		// 显示发动解冻，紧接着显示目标解冻
		ui.battleEdit->append(QString::fromStdString(action->getInitiator()->getName()) + " <font color='blueviolet'>解冻</font>了");
		break;
	}
	QCoreApplication::processEvents();				// 手动处理事件循环
	QThread::currentThread()->msleep(sleepTime);	// 线程休眠
}

void GameWindow::GameEnd(std::vector<Player*> rank)
{
	QString html;
	// 表格宽度设为填充文本框宽度
	html.append("<table border='1' style='width:100%; text-align:center;'>");
	html.append("<tr><th>排名</th><th>名字</th><th>得分</th><th>GPA</th><th>击杀数</th><th>致命一击</th></tr>");
	for (int i = 0; i < static_cast<int>(rank.size()); i++) {
		html.append("<tr>");
		html.append("<td>" + QString::number(i + 1) + "</td>");
		html.append("<td>" + QString::fromStdString(rank[i]->getName()) + "</td>");
		html.append("<td>" + QString::number(rank[i]->getExp()) + "</td>");
		html.append("<td>" + QString::number(rank[i]->getGPA()) + "</td>");
		html.append("<td>" + QString::number(rank[i]->getKillCount()) + "</td>");
		html.append("<td>" + (rank[i]->getKilledBy() ? QString::fromStdString(rank[i]->getKilledBy()->getName()) : "") + "</td>");
		html.append("</tr>");
	}
	html.append("</table>");
	// 将表格添加到battleEdit末尾
	ui.battleEdit->append(html);
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
			// 连接玩家的 hpmaxChanged 信号到 QProgressBar 的 setRange 槽，以便在玩家生命值上限改变时自动更新血条范围
			connect(player, &Player::hpmaxChanged, healthBar, &QProgressBar::setRange);

			ui.statusLayout->addWidget(playerName);
			ui.statusLayout->addWidget(healthBar);
		}
		if (i < game->getTeamCount() - 1)
			ui.statusLayout->addItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 添加占位符以空出一定距离
	}
	// 显示分割线分隔后面对战信息
	ui.battleEdit->append("------------------------------------------------------------");
	QCoreApplication::processEvents();		// 手动处理事件循环
}

void GameWindow::on_fastButton_clicked()
{
	sleepTime = 100;						// 设置间隔时间为100ms
}

void GameWindow::closeEvent(QCloseEvent* event)
{
	event->accept();
	delete this;
}
