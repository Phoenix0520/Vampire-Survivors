#pragma once

class SceneManager
{
private: // ���� �Ҹ���
	SceneManager(); 
	~SceneManager();
	SceneManager(const SceneManager* other) {};

public:	// �Լ�
	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}
	// �̱��� �ν��Ͻ� �Լ�

	void Update();
	void Render();

	void ThreadStart1();
	void ThreadStart2();
	void RenderChapterDisplay();
	// Thread �� �������� �Լ�

	void ChangeScene(string name);


	bool IsCompleteCreate() { return complete; }

	class Scene* GetScene(string name);
	class Scene* GetCurScene() { return curScene; }
	class Scene* GetLatScene() { return latScene; }
	// Getter �Լ�

private: // ����� �Լ�
	void CreateObject();

private: // private �Լ�
	vector<class Scene*> scenes;	

	class Scene* curScene = nullptr;
	class Scene* latScene = nullptr;

	int curSceneNum = 0;
	int latSceneNum = 0;

	bool complete = false;
};


