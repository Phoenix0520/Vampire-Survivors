#pragma once

class MobManager	// 추상클래스
{
private:	// 생성자, 소멸자
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

public:	// 순수 가상함수
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
