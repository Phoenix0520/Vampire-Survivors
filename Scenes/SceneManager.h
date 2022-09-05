#pragma once

class SceneManager
{
private: // 생성 소멸자
	SceneManager(); 
	~SceneManager();
	SceneManager(const SceneManager* other) {};

public:	// 함수
	void Update();
	void Render();
	// MainWindow 에서 Call 이된다.

	void ThreadStart1();
	void ThreadStart2();
	void RenderChapterDisplay();
	// Thread 를 쓰기위한 함수

	void ChangeScene(string name);

	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}
	// 싱글톤 인스턴스 함수

	bool IsCompleteCreate() { return complete; }

	class Scene* GetScene(string name);
	class Scene* GetCurScene() { return curScene; }
	class Scene* GetLatScene() { return latScene; }
	// Getter 함수

private: // 비공유 함수
	void CreateObject();

private: // private 함수
	vector<class Scene*> scenes;	// Scene에 대한 데이터를 저장한다.
	// 이럴경우 주소의 배열이 vector 에 들어가는것이다.
	
	class Scene* curScene = nullptr;
	class Scene* latScene = nullptr;

	int curSceneNum = 0;
	int latSceneNum = 0;

	bool complete = false;
};


