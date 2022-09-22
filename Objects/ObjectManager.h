#pragma once

class ObjectManager	// �߻�Ŭ����
{
private:	// ������, �Ҹ���
	ObjectManager();
	~ObjectManager();

public:
	static ObjectManager* GetInstance()
	{
		static ObjectManager instance;

		return &instance;
	}

public:	// ���� �����Լ�
	void		Update(string name, Matrix V, Matrix P);
	void		Render(string name);

	void		UpdateAll(Matrix V, Matrix P);
	void		RenderAll();

	void		AddObject(string name, class GameObject* obj);
	void		AddObjectStrings(string name)				{ objectStrings.push_back(name); }
	void		ClearObjectStrings()						{ objectStrings.clear(); }

	void SetPosition(string name, Vector2 pos);
	void SetPosition(string name, float x, float y);

	class GameObject* FindObject(string name);

private:
	map<string, class GameObject*> objects;
	vector<string> objectStrings;
};