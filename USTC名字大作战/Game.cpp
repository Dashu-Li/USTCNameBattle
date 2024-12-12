#include <algorithm>
#include <random>
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
	bool isCritical = rand() % 100 < defender->getLevel();		// 暴击率为发动者等级
	bool isMiss = rand() % 100 < attacker->getLevel();			// 闪避率为目标等级
	int damage = attacker->getAtk() - defender->getDef();
	if (damage < 0 || isMiss) damage = 0;
	if (isCritical) damage *= 2;
	defender->addHp(-damage);
	return new Action(Action::Attack, attacker, defender, damage, isCritical, isMiss);
}

Action* Game::heal(Player* healer, Player* target)
{
	// TODO: 实现治疗逻辑
	return new Action(Action::Heal, healer, target);
}

Action* Game::skill(Player* caster, Player* target)
{
	// TODO: 实现技能逻辑
	return new Action(Action::Skill, caster, target);
}

Action::Action(ActionType actiontype, Player* initiator, Player* target, int damage, bool isCritical, bool isMiss) :
	actiontype(actiontype), initiator(initiator), target(target), damage(damage), isCritical(isCritical), isMiss(isMiss)
{}

const Action::ActionType& Action::getActionType() const { return actiontype; }

const int& Action::getDamage() const { return damage; }

const bool& Action::getIsCritical() const { return isCritical; }

const bool& Action::getIsMiss() const { return isMiss; }

Player::Player(std::string name, int hp, int atk, int def, int GPA, int exp, int level)
{
	this->name = name;
	this->hp = hp;
	this->atk = atk;
	this->def = def;
	this->GPA = GPA;
	this->exp = exp;
	this->level = level;
}

const std::string& Player::getName() const { return name; }

const int& Player::getHp() const { return hp; }

const int& Player::getAtk() const { return atk; }

const int& Player::getDef() const { return def; }

const int& Player::getGPA() const { return GPA; }

const int& Player::getExp() const { return exp; }

const int& Player::getLevel() const { return level; }

bool Player::isDead() const { return hp <= 0; }

void Player::setHp(int hp) { this->hp = hp; emit hpChanged(hp); }

void Player::setAtk(int atk) { this->atk = atk; }

void Player::setDef(int def) { this->def = def; }

void Player::setGPA(int GPA) { this->GPA = GPA; }

void Player::setExp(int exp) { this->exp = exp; }

void Player::setLevel(int level) { this->level = level; }

int Player::levelUp() { level++; return level; }

int Player::addExp(int exp) { this->exp += exp; return exp; }

int Player::addGPA(int GPA) { this->GPA += GPA; return GPA; }

int Player::addHp(int hp) { emit hpChanged(this->hp += hp); return hp; }

int Player::addAtk(int atk) { this->atk += atk; return atk; }

int Player::addDef(int def) { this->def += def; return def; }