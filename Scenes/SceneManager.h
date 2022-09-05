#pragma once

class SceneManager
{
private: // ���� �Ҹ���
	SceneManager(); 
	~SceneManager();
	SceneManager(const SceneManager* other) {};

public:	// �Լ�
	void Update();
	void Render();
	// MainWindow ���� Call �̵ȴ�.

	void ThreadStart1();
	void ThreadStart2();
	void RenderChapterDisplay();
	// Thread �� �������� �Լ�

	void ChangeScene(string name);

	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}
	// �̱��� �ν��Ͻ� �Լ�

	bool IsCompleteCreate() { return complete; }

	class Scene* GetScene(string name);
	class Scene* GetCurScene() { return curScene; }
	class Scene* GetLatScene() { return latScene; }
	// Getter �Լ�

private: // ����� �Լ�
	void CreateObject();

private: // private �Լ�
	vector<class Scene*> scenes;	// Scene�� ���� �����͸� �����Ѵ�.
	// �̷���� �ּ��� �迭�� vector �� ���°��̴�.
	
	class Scene* curScene = nullptr;
	class Scene* latScene = nullptr;

	int curSceneNum = 0;
	int latSceneNum = 0;

	bool complete = false;
};


