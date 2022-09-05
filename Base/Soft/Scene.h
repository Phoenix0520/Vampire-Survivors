#pragma once
//////////////////////////////////////////////////////////////
//															//
// Scene 추상화 클래스									    //
// 순수 가상함수로만 이루어져있다.							    //
// Intro -> 1 ~ 9 -> Boss -> Outro						    //
// 이렇게 씬들을 상속받아서 사용하기 위해 추상 클래스로 만든다.	//
//														    //
// 1. virtual 로 정의된 함수가 있으면 추상화 크래스		    //
// 2. Delete 순서 알아보기 Child -> Parent					//
// 3. Private, Protect 알아보기								//
//															//
//////////////////////////////////////////////////////////////

class Scene
{
public:
	Scene() {};
	~Scene() {};

public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void ChangeScene() = 0; // Scene 을 넘길때 화면을 초기화하기위한 함수
	// 순수 가상함수, 상속 받은곳에서 반드시 구현 되어야된다.

	void		 SetActive(bool val) { active = val; }
	void		 SetNextScene(string name) { nextSceneName = name; }
	void		 SetReset(bool val) { reset = val; }
	void		 SetDisplay(bool val) { display = val; }
	// Setter

	bool		 IsActive() { return active; }
	bool		 GetReset() { return reset; }
	int			 GetDisplay() { return display; }
	string		 GetSceneName() { return sceneName; }
	string		 GetNextSceneName() { return nextSceneName; }
	// Getter

protected:
	bool   active = true;    // 해당 Scene 이 진행중 일 때는 건들지않고, false 일 때는 Update, Render 가 이루어지지 않는다.
	bool   reset  = true;

	int	   display = false;

	string sceneName;	   // 현재 Scene 의 명칭
	string nextSceneName; // 다음 신의 명칭

};

