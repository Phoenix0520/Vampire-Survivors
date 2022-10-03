#pragma once

class ObjectManager	// �̱��� ������ ����
{
private:
	ObjectManager();
	~ObjectManager();
	// ���������� �ܺ� ���� �� �Ҹ� �Ұ�

public:
	static ObjectManager* GetInstance()
	{
		static ObjectManager instance;
		return &instance;
	}
	// Singleton Instance ��ȯ

public:
	void Update(string name, Matrix V, Matrix P);
	void Render(string name);
	// �̸��� ���� Ư�� ������Ʈ�� �����Ͽ� ����, ����մϴ�.
	void UpdateAll(Matrix V, Matrix P);
	void RenderAll();
	// ����Ʈ�� �ִ� ��� ������Ʈ�� ����, ����մϴ�.
	void AddObject(string name, class GameObject* obj);
	// ������Ʈ ��ü�� �߰��մϴ�.
	void AddObjectStrings(string name) { objectStrings.push_back(name); }
	// ����Ʈ�� �̸��� �߰��մϴ�.
	void ClearObjectStrings() { objectStrings.clear(); }
	// ����Ʈ�� �ʱ�ȭ �մϴ�.
	void SetPosition(string name, Vector2 pos);
	void SetPosition(string name, float x, float y);
	// �̸��� ���� Ư�� ������Ʈ�� ��ġ�� �����մϴ�.
	class GameObject* FindObject(string name);
	// �̸��� ���� Ư�� ������Ʈ�� �����͸� ��ȯ�մϴ�.

private:
	map<string, class GameObject*> objects;
	// �̸��� �Բ� ���� ������Ʈ ��ü�� ����ϴ�.
	vector<string> objectStrings;
	// �Ѳ����� ����ؾ� �ϱ����� ����Ʈ �Դϴ�.

};