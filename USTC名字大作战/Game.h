#pragma once
#include <vector>
#include <string>

class Player {
private:
	std::string name;	// 名字
	int hp;				// 生命值
	int atk;			// 攻击力
	int def;			// 防御力
	int GPA;			// 绩点
	int exp;			// 经验值
	int level;			// 等级
public:
	Player(std::string name = "", int hp = 100, int atk = 10, int def = 5, int GPA = 4.3, int exp = 0, int level = 1);
	const std::string& getName() const;	// 获取名字
	const int& getHp() const;			// 获取生命值
	const int& getAtk() const;			// 获取攻击力
	const int& getDef() const;			// 获取防御力
	const int& getGPA() const;			// 获取绩点
	const int& getExp() const;			// 获取经验值
	const int& getLevel() const;		// 获取等级
	bool isDead() const;				// 是否死亡
	void setHp(int hp);					// 设置生命值
	void setAtk(int atk);				// 设置攻击力
	void setDef(int def);				// 设置防御力
	void setGPA(int GPA);				// 设置绩点
	void setExp(int exp);				// 设置经验值
	void setLevel(int level);			// 设置等级
	void levelUp();						// 升级
	void addExp(int exp);				// 增加经验值
	void addGPA(int GPA);				// 增加绩点
	void addHp(int hp);					// 增加生命值
	void addAtk(int atk);				// 增加攻击力
	void addDef(int def);				// 增加防御力
};

class Attack {
private:
	Player* attacker;	// 攻击者
	Player* defender;	// 防御者
	int damage;			// 伤害值
	bool isCritical;	// 是否暴击
	bool isMiss;		// 是否闪避
public:
	Attack(Player* attacker, Player* defender, int damage);
	const int& getDamage() const;		// 获取伤害值
	const bool& getIsCritical() const;	// 获取是否暴击
	const bool& getIsMiss() const;		// 获取是否闪避
};

class Game {
private:
	int teamCount;								// 队伍数量
	std::vector<std::vector<Player*>> teams;	// 队伍玩家列表
public:
	Game();
	const int& getTeamCount() const;							// 获取队伍数量
	const std::vector<std::vector<Player*>>& getTeams() const;	// 获取队伍玩家列表
	bool isTeamBattle() const;									// 是否为组队对战
	void addPlayer(std::string name, int team);					// 添加玩家
	// TODO: 添加玩法实现
	Attack attack(Player* attacker, Player* defender);			// 攻击
};