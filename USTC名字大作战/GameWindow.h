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
	QVBoxLayout* statusLayout;
	void displayStatus();		// 显示每个玩家生命条，使用HTML文本输出到statusBrowser

private slots:
	void on_battleStartButton_clicked();
	void on_PlayAgainButton_clicked();

signals:
	void Back();
};