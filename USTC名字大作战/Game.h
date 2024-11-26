#pragma once

class Player {
private:
	int hp;		// ����ֵ
	int atk;	// ������
	int def;	// ������
	int GPA;	// ����
	int exp;	// ����ֵ
	int level;	// �ȼ�
public:
	Player();
	Player(int hp, int atk, int def, int GPA, int exp, int level);
	int getHp() const;			// ��ȡ����ֵ
	int getAtk() const;			// ��ȡ������
	int getDef() const;			// ��ȡ������
	int getGPA() const;			// ��ȡ����
	int getExp() const;			// ��ȡ����ֵ
	int getLevel() const;		// ��ȡ�ȼ�
	void setHp(int hp);			// ��������ֵ
	void setAtk(int atk);		// ���ù�����
	void setDef(int def);		// ���÷�����
	void setGPA(int GPA);		// ���ü���
	void setExp(int exp);		// ���þ���ֵ
	void setLevel(int level);	// ���õȼ�
	void levelUp();				// ����
	void addExp(int exp);		// ���Ӿ���ֵ
	void addGPA(int GPA);		// ���Ӽ���
	void addHp(int hp);			// ��������ֵ
	void addAtk(int atk);		// ���ӹ�����
	void addDef(int def);		// ���ӷ�����
};