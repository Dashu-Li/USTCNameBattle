#pragma once

#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
#include "ui_GameWindow.h"

class GameWindow : public QMainWindow
{
	Q_OBJECT

public:
	GameWindow(QWidget *parent = nullptr);
	~GameWindow();

protected:
	void closeEvent(QCloseEvent *event) override;

private:
	Ui::GameWindowClass ui;

signals:
	void Back();
};