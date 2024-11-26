#pragma once

class Player {
private:
	int hp;		// 生命值
	int atk;	// 攻击力
	int def;	// 防御力
	int GPA;	// 绩点
	int exp;	// 经验值
	int level;	// 等级
public:
	Player();
	Player(int hp, int atk, int def, int GPA, int exp, int level);
	int getHp() const;			// 获取生命值
	int getAtk() const;			// 获取攻击力
	int getDef() const;			// 获取防御力
	int getGPA() const;			// 获取绩点
	int getExp() const;			// 获取经验值
	int getLevel() const;		// 获取等级
	void setHp(int hp);			// 设置生命值
	void setAtk(int atk);		// 设置攻击力
	void setDef(int def);		// 设置防御力
	void setGPA(int GPA);		// 设置绩点
	void setExp(int exp);		// 设置经验值
	void setLevel(int level);	// 设置等级
	void levelUp();				// 升级
	void addExp(int exp);		// 增加经验值
	void addGPA(int GPA);		// 增加绩点
	void addHp(int hp);			// 增加生命值
	void addAtk(int atk);		// 增加攻击力
	void addDef(int def);		// 增加防御力
};