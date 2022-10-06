#pragma once

class EntityManager	// ManagerClass
{
private:	// �̱��� ������ ����
	EntityManager();
	~EntityManager();

public:
	static EntityManager* GetInstance()
	{
		static EntityManager instance;

		return &instance;
	} // �̱��� �ν��Ͻ� ��ȯ

public:
	void Update(Matrix V, Matrix P);
	void Render();
	void ToggleVisible() { if (visible) visible = false; else visible = true; }
	// UI �� �����ִ� �Լ�

public:
	void UpdateAll();
	void UpdateMob();
	void UpdateItem();
	void UpdateBrazier(); 
	// ������ Pool�� ������Ʈ �մϴ�

	vector<class Monster*>	GetAvailMobs();
	vector<class DropItem*>	GetAvailItems();
	vector<class Brazier*>	GetAvailBraziers();	
	// Pool �ȿ� Ȱ��ȭ �Ǿ��ִ� ��ü�� �����͸� ��� �ֽ��ϴ�.

	UINT GetAddableMobIndex();
	UINT GetAddableItemIndex();
	UINT GetAddableBrazIndex();
	// �������� �Ҵ�� ��ȣ�� ��ȯ�մϴ�.

	UINT GetMobIndex(class Monster* mob);
	UINT GetItemIndex(class DropItem* item);
	UINT GetBrazIndex(class Brazier* brz);
	// ��ü�� �ּҸ� �Ű������� ���° ��ü���� �˾Ƴ��ϴ�.

	void AddMob		(string str, class Monster* obj)	{ mobs. push_back(make_pair(str, obj)); UpdateMob(); }
	void AddItem	(string str, class DropItem* obj)	{ items.push_back(make_pair(str, obj)); UpdateItem(); }
	void AddBrazier	(string str, class Brazier* obj)	{ brazs.push_back(make_pair(str, obj)); UpdateBrazier(); }
	// ��ü�� Ȱ��ȭ �����ִ� �Լ� �Դϴ�.

	void RemoveMob(UINT index);
	void RemoveItem(UINT index);
	void RemoveBrzier(UINT index);
	// ��ü�� ��Ȱ��ȭ �����ִ� �Լ� �Դϴ�.

private:
	vector<pair<string, class Monster*>>	mobs;
	vector<pair<string, class DropItem*>>	items;
	vector<pair<string, class Brazier*>>	brazs;
	// ������ vector�� �̸��� �Բ� �ּҰ��� ��� �ֽ��ϴ�.

	class Texture* background = nullptr;
	bool visible = false;

};