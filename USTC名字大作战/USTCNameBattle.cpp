#include "USTCNameBattle.h"

USTCNameBattle::USTCNameBattle(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

USTCNameBattle::~USTCNameBattle()
{}

void USTCNameBattle::on_pushButton_clicked()
{
	ui.statusBar->setAccessibleDescription("Hello, World!");
}