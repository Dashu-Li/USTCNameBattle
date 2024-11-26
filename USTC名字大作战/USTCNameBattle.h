#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_USTCNameBattle.h"
#include "GameWindow.h"

class USTCNameBattle : public QMainWindow
{
    Q_OBJECT

public:
    USTCNameBattle(QWidget *parent = nullptr);
    ~USTCNameBattle();

private:
    Ui::USTCNameBattleClass ui;
    GameWindow *gameWindow;

private slots:
	void GameStart();
    void Back();
};
