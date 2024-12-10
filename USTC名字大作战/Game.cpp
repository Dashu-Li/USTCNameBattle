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

Attack Game::attack(Player* attacker, Player* defender)
{
	int damage = attacker->getAtk() - defender->getDef();
	if (damage < 0) damage = 0;
	defender->setHp(defender->getHp() - damage);
	return Attack(attacker, defender, damage);
}

Attack::Attack(Player* attacker, Player* defender, int damage)
{
	this->attacker = attacker;
	this->defender = defender;
	this->damage = damage;
	isCritical = rand() % 100 < attacker->getLevel();
	isMiss = rand() % 100 < defender->getLevel();
}

const int& Attack::getDamage() const { return damage; }

const bool& Attack::getIsCritical() const { return isCritical; }

const bool& Attack::getIsMiss() const { return isMiss; }

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

void Player::setHp(int hp) { this->hp = hp; }

void Player::setAtk(int atk) { this->atk = atk; }

void Player::setDef(int def) { this->def = def; }

void Player::setGPA(int GPA) { this->GPA = GPA; }

void Player::setExp(int exp) { this->exp = exp; }

void Player::setLevel(int level) { this->level = level; }

void Player::levelUp() { level++; }

void Player::addExp(int exp) { this->exp += exp; }

void Player::addGPA(int GPA) { this->GPA += GPA; }

void Player::addHp(int hp) { this->hp += hp; }

void Player::addAtk(int atk) { this->atk += atk; }

void Player::addDef(int def) { this->def += def; }