#include <algorithm>
#include <random>
#include <ctime>
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

Action* Game::attack(Player* attacker, Player* defender)
{
	bool isCritical = rand() % 128 < defender->getCrit();
	bool isMiss = rand() % 128 < attacker->getMiss();
	int damage = attacker->getAtk() - defender->getDef();
	if (damage < 0 || isMiss) damage = 0;
	if (isCritical) damage *= 2;
	defender->addHp(-damage);
	return new Action(Action::Attack, attacker, defender, damage, isCritical, isMiss);
}

Action* Game::heal(Player* healer, Player* target)
{
	int heal = std::min( healer->getHeal() + target->getDef(), target->getHpMax() - target->getHp() );
	target->addHp(heal);
	return new Action(Action::Heal, healer, target, heal);
}
/*
Action* Game::skill(Player* caster, Player* target)
{
	// TODO: 实现技能逻辑
	return new Action(Action::Skill, caster, target);
}
*/         //先砍掉
Action::Action(ActionType actiontype, Player* initiator, Player* target, int damage, bool isCritical, bool isMiss) :
	actiontype(actiontype), initiator(initiator), target(target), damage(damage), isCritical(isCritical), isMiss(isMiss)
{}

const Action::ActionType& Action::getActionType() const { return actiontype; }

const int& Action::getDamage() const { return damage; }

const bool& Action::getIsCritical() const { return isCritical; }

const bool& Action::getIsMiss() const { return isMiss; }

Player::Player(std::string name) : name(name)
{
	// 使用hash算法按名字生成玩家属性，确保每个玩家的属性都是唯一的
	std::hash<std::string> hash;
	hp = hash(name + "hp") % 128 + 128;          // 血量: 128 - 255
	atk = hash(name + "atk") % 32 + 32;          // 攻击力: 32 - 63
	def = hash(name + "def") % 16 + 16;          // 防御力: 16 - 31
	crit = hash(name + "crit") % 8 + 8;          // 暴击率: ( 8 - 15 ) / 128      即约为 1/16 - 1/8
	miss = hash(name + "miss") % 8 + 8;          // 闪避率: ( 8 - 15 ) / 128      即约为 1/16 - 1/8
	heal = hash(name + "heal") & 16 + 16;        // 回复力: 16 - 31
	GPA = 0;
	exp = 0;
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

bool Player::isDead() const { return hp <= 0; }

void Player::setHp(int hp) { this->hp = hp; emit hpChanged(hp); }

void Player::setAtk(int atk) { this->atk = atk; }

void Player::setDef(int def) { this->def = def; }

void Player::setCrit(int crit) { this->crit = crit; }

void Player::setMiss(int miss) { this->miss = miss; }

void Player::setHeal(int heal) { this->heal = heal; }

void Player::setExp(int exp) { this->exp = exp; }

void Player::setGPA(int GPA) { this->GPA = GPA; }

int Player::addExp(int exp) { this->exp += exp; return exp; }

int Player::addGPA(int GPA) { this->GPA += GPA; return GPA; }

int Player::addHp(int hp) { emit hpChanged(this->hp += hp); return hp; }

int Player::addAtk(int atk) { this->atk += atk; return atk; }

int Player::addDef(int def) { this->def += def; return def; }

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
	int Playersalive, Teamsalive =  0;
	for (int i = 0; i < getTeamCount(); i++) {
		Playersalive = 0;
		for (int j = 0; j < teams[i].size(); j++) {
			Playersalive += 1 - teams[i][j]->isDead();
		}
		if (Playersalive) Teamsalive++;
	}
	return Teamsalive;
}

void Game::Regroup()
{
	if (getTeamCount() > 1) return;
	std::vector<Player*> temp;
	for (int i = 1; i < teams[0].size(); i++) {
		temp.clear(); temp.push_back(teams[0][i]);
		teams.push_back(temp);
	}
	temp.clear(); temp.push_back(teams[0][0]); teams[0].clear(); teams[0].push_back(temp[0]);
}

Action Game::GenerateAttack()
{
	// 刷新权重
	for (int i = 0; i < getTeamCount(); i++)
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead()) {
				weight[i][j].attacker = std::max(PlayersAlive(), weight[i][j].attacker + 1);
				weight[i][j].defender = std::max(PlayersAlive(), weight[i][j].defender + 1);
			}

	int total_attacker = 0, total_defender = 0, attacker_choosed = 0, defender_choosed = 0, temp;
	int attacker_i, attacker_j, defender_i, defender_j;

	// 按权重随机产生攻击者
	for (int i = 0; i < getTeamCount(); i++)
		for (int j = 0; j < teams[i].size(); j++)
			if (!teams[i][j]->isDead() && weight[i][j].attacker > 0)
				total_attacker += weight[i][j].attacker;
	attacker_choosed = rand() % total_attacker; temp = 0;                // 将所有人权重相加，随机生成一个数，看他落在哪个人的前缀和之内
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
		if (i == attacker_i) continue;                        // 避免友伤
		for (int j = 0; j < getTeamCount(); j++)
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

	// 计算伤害
	bool isCritical = rand() % 128 < teams[attacker_i][attacker_j]->getCrit();
	bool isMiss = rand() % 128 < teams[defender_i][defender_j]->getMiss();
	int damage = teams[attacker_i][attacker_j]->getAtk() - teams[defender_i][defender_j]->getDef();
	if (damage < 0 || isMiss) damage = 0;
	if (isCritical) damage *= 2;
	teams[defender_i][defender_j]->addHp(-damage);

	return Action(Action::Attack, teams[attacker_i][attacker_j], teams[defender_i][defender_j], damage, isCritical, isMiss);
}

void Game::GenerateGame()
{
	srand((unsigned)time(NULL));                  //刷新随机种子

	Regroup();
	//初始化每个玩家的行动权重，初始值为存活玩家数，每项行动完成后，该行动权重改为1
	//std::vector<std::vector<Weight>> weight;
	weight.resize(getTeamCount());
	for (int i = 0; i < getTeamCount(); i++)
		for (int j = 0; j < teams[i].size(); j++)
			weight[i].emplace_back(PlayersAlive(), PlayersAlive(), PlayersAlive(), PlayersAlive());

	std::vector<Action> progress;
	while(TeamsAlive() > 1) {
		int seed = rand() % 10;
		switch (seed) {
			case 0:
			case 1:
			case 2:
			case 3: {
				progress.push_back(GenerateAttack());
			}
			case 4:
			case 5:
			case 6:
			case 7: {
			}
		}
	}
}