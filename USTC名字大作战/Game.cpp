#include "Game.h"

Game::Game() : teamCount(0) {}

int Game::getTeamCount() const { return teamCount; }

bool Game::isTeamBattle() const { return teamCount > 1; }

void Game::addPlayer(std::string name, int team)
{
	if (team >= teamCount)
	{
		teamCount = team + 1;
		teams.resize(teamCount);
	}
	teams[team].push_back(new Player(name));
}

Attack Game::attack(Player* attacker, Player* defender)
{
	int damage = attacker->getAtk() - defender->getDef();
	if (damage < 0)
		damage = 0;
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

int Attack::getDamage() const { return damage; }

bool Attack::getIsCritical() const { return isCritical; }

bool Attack::getIsMiss() const { return isMiss; }

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

int Player::getHp() const { return hp; }

int Player::getAtk() const { return atk; }

int Player::getDef() const { return def; }

int Player::getGPA() const { return GPA; }

int Player::getExp() const { return exp; }

int Player::getLevel() const { return level; }

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