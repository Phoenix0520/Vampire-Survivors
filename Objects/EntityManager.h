#pragma once

class EntityManager
{
private:
	EntityManager();
	~EntityManager();

public:
	static EntityManager* GetInstance()
	{
		static EntityManager instance;

		return &instance;
	}

public:
	void Update(Matrix V, Matrix P);
	void Render();
	void ToggleVisible() { if (visible) visible = false; else visible = true; }

public:
	void UpdateAll();
	void UpdateMob();
	void UpdateItem();
	void UpdateBrazier();

	vector<class Monster*>	GetAvailMobs();
	vector<class DropItem*>	GetAvailItems();
	vector<class Brazier*>	GetAvailBraziers();

	UINT GetAddableMobIndex();
	UINT GetAddableItemIndex();
	UINT GetAddableBrazIndex();

	UINT GetMobIndex(Monster* mob);
	UINT GetItemIndex(DropItem* item);
	UINT GetBrazIndex(Brazier* brz);

	void AddMob		(string str, Monster* obj)		{ mobs. push_back(make_pair(str, obj)); UpdateMob(); }
	void AddItem	(string str, DropItem* obj)		{ items.push_back(make_pair(str, obj)); UpdateItem(); }
	void AddBrazier	(string str, Brazier* obj)		{ brazs.push_back(make_pair(str, obj)); UpdateBrazier(); }

	void RemoveMob(UINT index);
	void RemoveItem(UINT index);
	void RemoveBrzier(UINT index);

	void SetAddPos(Vector2 pos) { addPos = pos; }

private:
	vector<pair<string, class Monster*>>	mobs;
	vector<pair<string, class DropItem*>>	items;
	vector<pair<string, class Brazier*>>	brazs;

	class Texture* background = nullptr;

	bool visible = false;

	Vector2 addPos = Vector2(0.0f, 0.0f);

};