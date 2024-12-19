#pragma once
#include <vector>
#include <string>
#include <QObject>
#include <QThread>

class Player : public QObject {
	Q_OBJECT

private:
	std::string name;	// 名字
	int hpmax;          // 生命值上限
	int hp;				// 生命值
	int atk;			// 攻击力
	int def;			// 防御力
	int crit;           // 暴击率
	int miss;           // 闪避率
	int heal;           // 回复力
	Player* firedBy;	// 被谁点燃
	bool isFrozen;		// 是否被冰冻
	int exp;			// 经验值
	double GPA;			// 绩点
	int killCount;		// 击杀数
	Player* killedBy;	// 被谁杀死

public:
	Player(std::string name = "");
	const std::string& getName() const;	// 获取名字
	const int& getHpMax() const;	    // 获取生命值上限
	const int& getHp() const;			// 获取生命值
	const int& getAtk() const;			// 获取攻击力
	const int& getDef() const;			// 获取防御力
	const int& getGPA() const;			// 获取绩点
	const int& getExp() const;			// 获取经验值
	const int& getCrit() const;			// 获取暴击率
	const int& getMiss() const;			// 获取闪避率
	const int& getHeal() const;			// 获取回复力
	Player* getFiredBy();				// 获取被谁点燃
	const int& getIsFrozen() const;		// 获取是否冰冻
	const int& getKillCount() const;	// 获取击杀数
	const Player* getKilledBy() const;	// 获取被谁杀死

	bool isDead() const;				// 是否死亡
	void setHp(int hp);					// 设置生命值
	void setAtk(int atk);				// 设置攻击力
	void setDef(int def);				// 设置防御力
	void setCrit(int crit);             // 设置暴击率
	void setMiss(int miss);             // 设置闪避率
	void setHeal(int heal);             // 设置回复力
	void setFiredBy(Player* player);	// 设置被谁点燃
	void setFrozen(bool yesorno);		// 设置是否被冰冻
	void setExp(int exp);				// 设置经验值
	void setGPA(int GPA);				// 设置绩点
	void setKilledBy(Player* player);	// 设置被谁杀死

	int addExp(int exp);				// 增加经验值
	int addGPA(int GPA);				// 增加绩点
	int addHp(int hp);					// 增加生命值
	int addHpMax(int addhpmax);			// 增加生命值上限
	int addAtk(int atk);				// 增加攻击力
	int addDef(int def);				// 增加防御力
	int addCrit(int crit);				// 增加暴击率
	int addMiss(int miss);				// 增加闪避率
	int addHeal(int heal);				// 增加回复力
	int addKillCount();					// 击杀数加一

	bool operator<(const Player& other) const {		// 重载<运算符
		return (isDead() < other.isDead()) || (isDead() == other.isDead() && getExp() > other.getExp());		// 按照 1.是否存活；2.经验值 升序排列，GPA高的“比较小”，排在最前面
	}

signals:
	void hpChanged(int newHp);			// 生命值改变信号

};

class Action {
public:
	// 操作类型
	enum ActionType {
		Attack,			// 攻击
		Heal,			// 治疗
		Lifesteal,		// 吸血
		Ascension,		// 飞升
		Fire,			// 点燃
		Freeze,			// 冰冻
		Burn,			// 燃烧
		Extinguish,		// 熄灭
		Unfreeze		// 解冻
	};

	// 构造函数，返回一个操作对象，包含操作类型、发动者、目标、数值、是否暴击、是否闪避等信息
	Action(ActionType actiontype, Player* initiator, Player* target = nullptr, int damage = 0, int heal = 0, bool isCritical = false, bool isMiss = false);
	const ActionType& getActionType() const;	// 获取操作类型
	const Player* getInitiator() const;			// 获取发动者
	const Player* getTarget() const;			// 获取目标
	const int& getDamage() const;				// 获取伤害值
	const int& getHeal() const;					// 获取回复值
	const bool& getIsCritical() const;			// 获取是否暴击
	const bool& getIsMiss() const;				// 获取是否闪避

private:
	ActionType actiontype;	// 操作类型
	Player* initiator;		// 发动者
	Player* target;			// 目标
	int damage;				// 伤害值
	int heal;				// 回复值
	bool isCritical;		// 是否暴击
	bool isMiss;			// 是否闪避

};

struct Weight {             // 行动权重
	int attacker, defender, healer, healee;
	// 构造函数，方便初始化
	Weight(int a, int d, int h, int he) : attacker(a), defender(d), healer(h), healee(he) {}
};

class Game : public QObject {
	Q_OBJECT

private:
	int teamCount;								// 队伍数量
	std::vector<std::vector<Player*>> teams;	// 队伍玩家列表
	std::vector<std::vector<Weight>> weight;	// 行动权重
public:
	Game();
	const int& getTeamCount() const;							// 获取队伍数量
	const std::vector<std::vector<Player*>>& getTeams() const;	// 获取队伍玩家列表
	bool isTeamBattle() const;									// 是否为组队对战
	void addPlayer(std::string name, int team);					// 添加玩家

	int PlayersAlive() const;									// 存活玩家数量
	int TeamsAlive() const;										// 存活队伍数量
	void Regroup();												// 如果玩家未分组，即只有一组，重新分组为一人一组

	// TODO: 定义一个函数，返回一个保存了一局游戏所有操作的vector，每一回合中，按随机顺序遍历所有玩家，随机产生该玩家本回合操作，若攻击则随机选择一个敌方玩家进行攻击
	void BurnJudgement(std::vector<Action*> progress);			// 每回合开始时对所有人的着火状态判断
	void FrozenJudgement(std::vector<Action*> progress);		// 每回合开始时对所有人的冰冻状态判断

	Action* GenerateAttack();									// 普通攻击
	Action* GenerateHeal();										// 治疗
	Action* GenerateLifesteal();								// 吸血，实质是没有暴击的普通攻击，同时让吸血者回复造成伤害一半的血量
	Action* GenerateAscension();								// 飞升，即所有属性小幅增长
	void GenerateFire(std::vector<Action*> progress);			// 纵火，不造成伤害，但在接下来几回合会持续燃烧，概率灭火
	void GenerateFreeze(std::vector<Action*> progress);			// 冰冻，不造成伤害，使目标接下来几回合无法作为行动的发起者（普攻、治疗、吸血等）
	std::vector<Player*> CalculateGPA();						// 根据对局表现计算GPA
	void GenerateGame();										// 生成对局

signals:
	void generateAction(Action*);								// 产生操作信号
	void gameEnd();												// 游戏结束信号

};