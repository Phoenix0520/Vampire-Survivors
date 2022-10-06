#pragma once

class EntityManager	// ManagerClass
{
private:	// 싱글톤 디자인 패턴
	EntityManager();
	~EntityManager();

public:
	static EntityManager* GetInstance()
	{
		static EntityManager instance;

		return &instance;
	} // 싱글톤 인스턴스 반환

public:
	void Update(Matrix V, Matrix P);
	void Render();
	void ToggleVisible() { if (visible) visible = false; else visible = true; }
	// UI 를 보여주는 함수

public:
	void UpdateAll();
	void UpdateMob();
	void UpdateItem();
	void UpdateBrazier(); 
	// 각각의 Pool을 업데이트 합니다

	vector<class Monster*>	GetAvailMobs();
	vector<class DropItem*>	GetAvailItems();
	vector<class Brazier*>	GetAvailBraziers();	
	// Pool 안에 활성화 되어있는 객체의 포인터를 담고 있습니다.

	UINT GetAddableMobIndex();
	UINT GetAddableItemIndex();
	UINT GetAddableBrazIndex();
	// 다음으로 할당될 번호를 반환합니다.

	UINT GetMobIndex(class Monster* mob);
	UINT GetItemIndex(class DropItem* item);
	UINT GetBrazIndex(class Brazier* brz);
	// 객체의 주소를 매개변수로 몇번째 객체인지 알아냅니다.

	void AddMob		(string str, class Monster* obj)	{ mobs. push_back(make_pair(str, obj)); UpdateMob(); }
	void AddItem	(string str, class DropItem* obj)	{ items.push_back(make_pair(str, obj)); UpdateItem(); }
	void AddBrazier	(string str, class Brazier* obj)	{ brazs.push_back(make_pair(str, obj)); UpdateBrazier(); }
	// 객체를 활성화 시켜주는 함수 입니다.

	void RemoveMob(UINT index);
	void RemoveItem(UINT index);
	void RemoveBrzier(UINT index);
	// 객체를 비활성화 시켜주는 함수 입니다.

private:
	vector<pair<string, class Monster*>>	mobs;
	vector<pair<string, class DropItem*>>	items;
	vector<pair<string, class Brazier*>>	brazs;
	// 각각의 vector에 이름과 함께 주소값을 담고 있습니다.

	class Texture* background = nullptr;
	bool visible = false;

};