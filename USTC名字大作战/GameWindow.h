#pragma once

#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
#include <QProgressBar>
#include "ui_GameWindow.h"
#include "Game.h"

class GameWindow : public QMainWindow
{
	Q_OBJECT

public:
	GameWindow(QWidget* parent = nullptr);
	~GameWindow();

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	Ui::GameWindowClass ui;
	Game* game;
	void displayStatus();		// 显示每个玩家生命条

private slots:
	void on_battleStartButton_clicked();
	void on_PlayAgainButton_clicked();
	void displayAction(Action* action);
	void GameEnd(std::vector<Player*>);

signals:
	void Back();

};