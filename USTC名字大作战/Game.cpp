﻿#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>   // 用于时间处理
#include <thread>   // 用于线程休眠
#include <vector>
#include <cmath>
#include "Game.h"

Game::Game() : teamCount(0) {}

const int& Game::getTeamCount() const { return teamCount; }

const std::vector<std::vector<Player*>>& Game::getTeams() const { return teams; }

bool Game::isTeamBattle() const { return teamCount > 1; }

void Game::addPlayer(std::string name, int team)
{
	if (team >= teamCount) {
		teamCount = team + 1;
		teams.resize(teamCount);
	}
	teams[team].push_back(new Player(name));
}

int Game::PlayersAlive() const
{
	int Playersalive = 0;
	for (int i = 0; i < getTeamCount(); i++) {
		for (int j = 0; j < teams[i].size(); j++) {
			Playersalive += 1 - teams[i][j]->isDead();
		}
	}
	return Playersalive;
}

int Game::TeamsAlive() const
{
	int Playersalive, Teamsalive = 0;
	for (int i = 0; i < teamCount; i++) {
		Playersalive = 0;
		for (int j = 0; j < teams[i].size(); j++)
			Playersalive += 1 - teams[i][j]->isDead();
		if (Playersalive) Teamsalive++;
	}
	return Teamsalive;
}

void Game::Regroup()
{
	for (int i = 1; i < teams[0].size(); i++)
		addPlayer(teams[0][i]->getName(), i);
	teams[0].resize(1);
}

Action* Game::GenerateAttack()
{
	// 刷新权重
	for (int i = 0; i < getTeamCount(); i++)
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead()) {
				weight[i][j].attacker = std::max(PlayersAlive(), weight[i][j].attacker + 1);
				weight[i][j].defender = std::max(PlayersAlive(), weight[i][j].defender + 1);
			}

	int total_attacker = 0, total_defender = 0, attacker_choosed = 0, defender_choosed = 0, temp = 0;
	int attacker_i = 0, attacker_j = 0, defender_i = 0, defender_j = 0;

	// 按权重随机产生攻击者
	for (int i = 0; i < getTeamCount(); i++)
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead() && weight[i][j].attacker > 0)
				total_attacker += weight[i][j].attacker;
	attacker_choosed = rand() % total_attacker; temp = 0;				// 将所有人权重相加，随机生成一个数，看他落在哪个人的前缀和之内
	for (int i = 0; i < getTeamCount(); i++) {
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead() && weight[i][j].attacker > 0) {
				if (temp <= attacker_choosed && temp + weight[i][j].attacker > attacker_choosed) { temp += weight[i][j].attacker; attacker_i = i; attacker_j = j; break; }
				temp += weight[i][j].attacker;
			}
		if (temp > attacker_choosed) break;
	}

	// 按权重随机产生防御者
	for (int i = 0; i < getTeamCount(); i++) {
		if (i == attacker_i) continue;									// 避免友伤
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead() && weight[i][j].defender > 0)
				total_defender += weight[i][j].defender;
	}
	defender_choosed = rand() % total_defender; temp = 0;
	for (int i = 0; i < getTeamCount(); i++) {
		if (i == attacker_i) continue;
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead() && weight[i][j].defender > 0) {
				if (temp <= defender_choosed && temp + weight[i][j].defender > defender_choosed) { temp += weight[i][j].defender; defender_i = i; defender_j = j; break; }
				temp += weight[i][j].defender;
			}
		if (temp > defender_choosed) break;
	}

	// 重置攻击者和防御者的权重
	weight[attacker_i][attacker_j].attacker = 0;
	weight[defender_i][defender_j].defender = 0;

	// 计算伤害
	bool isCritical = rand() % 128 < teams[attacker_i][attacker_j]->getCrit();
	bool isMiss = rand() % 128 < teams[defender_i][defender_j]->getMiss();
	int damage = teams[attacker_i][attacker_j]->getAtk() - teams[defender_i][defender_j]->getDef();
	if (damage < 0 || isMiss) damage = 0;
	if (isCritical) damage *= 2;
	teams[defender_i][defender_j]->addHp(-damage);
	if (teams[defender_i][defender_j]->isDead()) {
		teams[attacker_i][attacker_j]->addKillCount();
		teams[defender_i][defender_j]->setKilledBy(teams[attacker_i][attacker_j]);
	}

	// 计算经验
	teams[attacker_i][attacker_j]->addExp(damage);

	Action* action = new Action(Action::Attack, teams[attacker_i][attacker_j], teams[defender_i][defender_j], damage, 0, isCritical, isMiss);
	emit generateAction(action);
	return action;
}

Action* Game::GenerateHeal()
{
	// 刷新权重
	for (int i = 0; i < getTeamCount(); i++)
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead()) {
				weight[i][j].healer = std::max(PlayersAlive(), weight[i][j].healer + 1);
				weight[i][j].healee = std::max(PlayersAlive(), weight[i][j].healee + 1);
			}

	int total_healer = 0, total_healee = 0, healer_choosed = 0, healee_choosed = 0, temp = 0;
	int healer_i = 0, healer_j = 0, healee_i = 0, healee_j = 0;

	// 按权重随机产生治疗者
	for (int i = 0; i < getTeamCount(); i++)
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead() && weight[i][j].healer > 0)
				total_healer += weight[i][j].healer;
	healer_choosed = rand() % total_healer; temp = 0;
	for (int i = 0; i < getTeamCount(); i++) {
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead() && weight[i][j].healer > 0) {
				if (temp <= healer_choosed && temp + weight[i][j].healer > healer_choosed) { temp += weight[i][j].healer; healer_i = i; healer_j = j; break; }
				temp += weight[i][j].healer;
			}
		if (temp > healer_choosed) break;
	}

	// 按权重随机产生被治疗者
	healee_i = healer_i;
	for (int j = 0; j < teams[healee_i].size(); j++)
		if (!teams[healee_i][j]->isDead() && weight[healee_i][j].healee > 0)
			total_healee += weight[healee_i][j].healee;
	healee_choosed = rand() % total_healee; temp = 0;
	for (int j = 0; j < teams[healee_i].size(); j++)
		if (!teams[healee_i][j]->isDead() && weight[healee_i][j].healee > 0) {
			if (temp <= healee_choosed && temp + weight[healee_i][j].healee > healee_choosed) { healee_j = j; break; }
			temp += weight[healee_i][j].healee;
		}

	// 重置治疗者和被治疗者的权重
	weight[healer_i][healer_j].healer = 0;
	weight[healee_i][healee_j].healee = 0;

	// 计算回血
	int heal = std::min(teams[healee_i][healee_j]->getHpMax() - teams[healee_i][healee_j]->getHp(), teams[healer_i][healer_j]->getHeal() + teams[healee_i][healee_j]->getDef());
	teams[healee_i][healee_j]->addHp(heal);

	// 计算经验
	teams[healer_i][healer_j]->addExp(heal);

	Action* action = new Action(Action::Heal, teams[healer_i][healer_j], teams[healee_i][healee_j], 0, heal);
	emit generateAction(action);
	return action;
}

Action* Game::GenerateLifesteal()
{
	// 刷新权重
	for (int i = 0; i < getTeamCount(); i++)
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead()) {
				weight[i][j].attacker = std::max(PlayersAlive(), weight[i][j].attacker + 1);
				weight[i][j].defender = std::max(PlayersAlive(), weight[i][j].defender + 1);
			}

	int total_attacker = 0, total_defender = 0, attacker_choosed = 0, defender_choosed = 0, temp = 0;
	int attacker_i = 0, attacker_j = 0, defender_i = 0, defender_j = 0;

	// 按权重随机产生攻击者
	for (int i = 0; i < getTeamCount(); i++)
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead() && weight[i][j].attacker > 0)
				total_attacker += weight[i][j].attacker;
	attacker_choosed = rand() % total_attacker; temp = 0;
	for (int i = 0; i < getTeamCount(); i++) {
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead() && weight[i][j].attacker > 0) {
				if (temp <= attacker_choosed && temp + weight[i][j].attacker > attacker_choosed) { temp += weight[i][j].attacker; attacker_i = i; attacker_j = j; break; }
				temp += weight[i][j].attacker;
			}
		if (temp > attacker_choosed) break;
	}

	// 按权重随机产生防御者
	for (int i = 0; i < getTeamCount(); i++) {
		if (i == attacker_i) continue;									// 避免友伤
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead() && weight[i][j].defender > 0)
				total_defender += weight[i][j].defender;
	}
	defender_choosed = rand() % total_defender; temp = 0;
	for (int i = 0; i < getTeamCount(); i++) {
		if (i == attacker_i) continue;
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead() && weight[i][j].defender > 0) {
				if (temp <= defender_choosed && temp + weight[i][j].defender > defender_choosed) { temp += weight[i][j].defender; defender_i = i; defender_j = j; break; }
				temp += weight[i][j].defender;
			}
		if (temp > defender_choosed) break;
	}

	// 重置攻击者和防御者的权重
	weight[attacker_i][attacker_j].attacker = 0;
	weight[defender_i][defender_j].defender = 0;

	// 计算伤害
	bool isMiss = rand() % 128 < teams[defender_i][defender_j]->getMiss();
	int damage = teams[attacker_i][attacker_j]->getAtk() - teams[defender_i][defender_j]->getDef();
	if (damage < 0 || isMiss) damage = 0;
	teams[defender_i][defender_j]->addHp(-damage);
	if (teams[defender_i][defender_j]->isDead()) {
		teams[attacker_i][attacker_j]->addKillCount();
		teams[defender_i][defender_j]->setKilledBy(teams[attacker_i][attacker_j]);
	}

	// 计算回血
	int heal = std::min(damage / 2, teams[attacker_i][attacker_j]->getHpMax() - teams[attacker_i][attacker_j]->getHp());
	if (isMiss) heal = 0;
	teams[attacker_i][attacker_j]->addHp(heal);

	// 计算经验
	teams[attacker_i][attacker_j]->addExp(damage + heal);

	Action* action = new Action(Action::Lifesteal, teams[attacker_i][attacker_j], teams[defender_i][defender_j], damage, heal, 0, isMiss);
	//emit generateAction(action);
	return action;
}

Action* Game::GenerateAscension()
{
	// 在存活玩家中随机选一个
	int choosed = rand() % PlayersAlive();
	int choosed_i = 0, choosed_j = 0;
	for (int i = 0; i < getTeamCount(); i++) {
		for (int j = 0; j < teams[i].size(); j++) {
			if (teams[i][j]->isDead()) continue;
			choosed--;
			if (choosed < 0) { choosed_j = j; break; }
		}
		if (choosed < 0) { choosed_i = i; break; }
	}

	// 将 生命值上限、攻击力、防御力、暴击率、闪避率、回复力 随机小幅度增加
	teams[choosed_i][choosed_j]->addHpMax(rand() % 32);
	teams[choosed_i][choosed_j]->addAtk(rand() % 8);
	teams[choosed_i][choosed_j]->addDef(rand() % 4);
	teams[choosed_i][choosed_j]->addCrit(rand() % 2);
	teams[choosed_i][choosed_j]->addMiss(rand() % 2);
	teams[choosed_i][choosed_j]->addHeal(rand() % 4);

	Action* action = new Action(Action::Ascension, teams[choosed_i][choosed_j]);
	//emit generateAction(action);
	return action;
}

std::vector<Player*> Game::CalculateGPA() {
	std::vector<Player*> rank;	// 按照结果排名，计算GPA
	for (int i = 0; i < getTeamCount(); i++)
		for (int j = 0; j < teams[i].size(); j++)
			rank.push_back(teams[i][j]);
	sort(rank.begin(), rank.end());

	double gpa = 4.3;
	for (int i = 0; i < rank.size(); i++) {
		if (rank[i]->isDead()) gpa -= 0.3;
		if (gpa - 1.0 < -0.000001) gpa = 0;
		rank[i]->setGPA(gpa);
	}
	return rank;
}

void Game::GenerateGame()
{
	srand((unsigned)time(NULL));                  //刷新随机种子

	// Regroup();
	// 初始化每个玩家的行动权重，初始值为存活玩家数，每项行动完成后，该行动权重改为1
	weight.resize(getTeamCount());
	for (int i = 0; i < getTeamCount(); i++)
		for (int j = 0; j < teams[i].size(); j++)
			weight[i].push_back(Weight(PlayersAlive(), PlayersAlive(), PlayersAlive(), PlayersAlive()));

	std::vector<Action*> progress;
	while (TeamsAlive() > 1) {
		int seed = rand() % 8;
		switch (seed) {
			case 0:
			case 1:
			case 2:
			case 3:
				progress.push_back(GenerateAttack());
				break;
			case 4:
			case 5:
				progress.push_back(GenerateHeal());
				break;
			case 6:
				progress.push_back(GenerateLifesteal());
				break;
			case 7:
				progress.push_back(GenerateAscension());
				break;
		}
		// 暂停
		// std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	std::vector<Player*> rank = CalculateGPA();
}

Action::Action(ActionType actiontype, Player* initiator, Player* target, int damage, int heal, bool isCritical, bool isMiss) :
	actiontype(actiontype), initiator(initiator), target(target), damage(damage), heal(heal), isCritical(isCritical), isMiss(isMiss)
{}

const Action::ActionType& Action::getActionType() const { return actiontype; }

const Player* Action::getInitiator() const { return initiator; }

const Player* Action::getTarget() const { return target; }

const int& Action::getDamage() const { return damage; }

const int& Action::getHeal() const { return heal; }

const bool& Action::getIsCritical() const { return isCritical; }

const bool& Action::getIsMiss() const { return isMiss; }

Player::Player(std::string name) :
	name(name), GPA(0), exp(0), killCount(0), killedBy(nullptr)
{
	// 使用hash算法按名字生成玩家属性，确保每个玩家的属性都是唯一的
	std::hash<std::string> hash;
	hp = hpmax = hash(name + "hp") % 128 + 128;		// 血量: 128 - 255
	atk = hash(name + "atk") % 32 + 32;				// 攻击力: 32 - 63
	def = hash(name + "def") % 16 + 16;				// 防御力: 16 - 31
	crit = hash(name + "crit") % 8 + 8;				// 暴击率: ( 8 - 15 ) / 128	即约为 1/16 - 1/8
	miss = hash(name + "miss") % 8 + 8;				// 闪避率: ( 8 - 15 ) / 128	即约为 1/16 - 1/8
	heal = hash(name + "heal") % 16 + 16;			// 回复力: 16 - 31
}

const std::string& Player::getName() const { return name; }

const int& Player::getHp() const { return hp; }

const int& Player::getHpMax() const { return hpmax; }

const int& Player::getAtk() const { return atk; }

const int& Player::getDef() const { return def; }

const int& Player::getGPA() const { return GPA; }

const int& Player::getExp() const { return exp; }

const int& Player::getCrit() const { return crit; }

const int& Player::getMiss() const { return miss; }

const int& Player::getHeal() const { return heal; }

const int& Player::getKillCount() const { return killCount; }

const Player* Player::getKilledBy() const { return killedBy; }

bool Player::isDead() const { return hp <= 0; }

void Player::setHp(int hp) { this->hp = hp; emit hpChanged(hp); }

void Player::setAtk(int atk) { this->atk = atk; }

void Player::setDef(int def) { this->def = def; }

void Player::setCrit(int crit) { this->crit = crit; }

void Player::setMiss(int miss) { this->miss = miss; }

void Player::setHeal(int heal) { this->heal = heal; }

void Player::setExp(int exp) { this->exp = exp; }

void Player::setGPA(int GPA) { this->GPA = GPA; }

void Player::setKilledBy(Player* player) { killedBy = player; }

int Player::addExp(int exp) { this->exp += exp; return exp; }

int Player::addGPA(int GPA) { this->GPA += GPA; return GPA; }

int Player::addHp(int addhp) { emit hpChanged(hp += std::min(hpmax - hp, std::max(-hp, addhp))); return addhp; }

int Player::addHpMax(int addhpmax) { this->hpmax += addhpmax; return addhpmax; }

int Player::addAtk(int atk) { this->atk += atk; return atk; }

int Player::addDef(int def) { this->def += def; return def; }

int Player::addCrit(int crit) { this->crit += crit; return crit; }

int Player::addMiss(int miss) { this->miss += miss; return miss; }

int Player::addHeal(int heal) { this->heal += heal; return heal; }

int Player::addKillCount() { return ++killCount; }