#pragma once

class ObjectManager	// 싱글톤 디자인 패턴
{
private:
	ObjectManager();
	~ObjectManager();
	// 마찬가지로 외부 생성 및 소멸 불가

public:
	static ObjectManager* GetInstance()
	{
		static ObjectManager instance;
		return &instance;
	}
	// Singleton Instance 반환

public:
	void Update(string name, Matrix V, Matrix P);
	void Render(string name);
	// 이름을 통해 특정 오브젝트를 지정하여 구성, 출력합니다.
	void UpdateAll(Matrix V, Matrix P);
	void RenderAll();
	// 리스트에 있는 모든 오브젝트를 구성, 출력합니다.
	void AddObject(string name, class GameObject* obj);
	// 오브젝트 객체를 추가합니다.
	void AddObjectStrings(string name) { objectStrings.push_back(name); }
	// 리스트에 이름을 추가합니다.
	void ClearObjectStrings() { objectStrings.clear(); }
	// 리스트를 초기화 합니다.
	void SetPosition(string name, Vector2 pos);
	void SetPosition(string name, float x, float y);
	// 이름을 통해 특정 오브젝트의 위치를 조정합니다.
	class GameObject* FindObject(string name);
	// 이름을 통해 특정 오브젝트의 포인터를 반환합니다.

private:
	map<string, class GameObject*> objects;
	// 이름과 함께 게임 오브젝트 객체를 담습니다.
	vector<string> objectStrings;
	// 한꺼번에 출력해야 하기위한 리스트 입니다.

};