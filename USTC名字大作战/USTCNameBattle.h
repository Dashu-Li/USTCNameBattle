#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_USTCNameBattle.h"

class USTCNameBattle : public QMainWindow
{
    Q_OBJECT

public:
    USTCNameBattle(QWidget *parent = nullptr);
    ~USTCNameBattle();

private:
    Ui::USTCNameBattleClass ui;

private slots:
	void GameStart();
};
