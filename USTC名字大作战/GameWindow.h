#pragma once

#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
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

private slots:
	void on_battleStartButton_clicked();
	void on_PlayAgainButton_clicked();

signals:
	void Back();
};