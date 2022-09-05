#pragma once

class MobManager	// �߻�Ŭ����
{
private:	// ������, �Ҹ���
	MobManager();
	~MobManager();

public:
	enum mobNum
	{
		BAT = 0,

	};

public:
	static MobManager* GetInstance()
	{
		static MobManager instance;

		return &instance;
	}

public:	// ���� �����Լ�
	void Update(string name, Matrix V, Matrix P);
	void Render(string name);
		 
	void AddMob(string name, class Monster* mob);
		 
	void AddPosition(Vector2 pos);
	void AddPosition(float x, float y);

	void SetPosition(int ID, Vector2 pos);
	void SetPosition(int ID, float x, float y);

	Vector2 GetPosition(int ID) { if (mobPos.size() <= (UINT)ID) return Vector2(0.0f, 0.0f); else return mobPos[ID]; }

	class Monster* FindMob(string name);

private:
	map<string, Monster*>	mobs;
	vector<Vector2>	mobPos;

};
