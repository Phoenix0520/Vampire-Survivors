#pragma once
//////////////////////////////////////////////////////////////
//															//
// Scene �߻�ȭ Ŭ����									    //
// ���� �����Լ��θ� �̷�����ִ�.							    //
// Intro -> 1 ~ 9 -> Boss -> Outro						    //
// �̷��� ������ ��ӹ޾Ƽ� ����ϱ� ���� �߻� Ŭ������ �����.	//
//														    //
// 1. virtual �� ���ǵ� �Լ��� ������ �߻�ȭ ũ����		    //
// 2. Delete ���� �˾ƺ��� Child -> Parent					//
// 3. Private, Protect �˾ƺ���								//
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
	virtual void ChangeScene() = 0; // Scene �� �ѱ涧 ȭ���� �ʱ�ȭ�ϱ����� �Լ�
	// ���� �����Լ�, ��� ���������� �ݵ�� ���� �Ǿ�ߵȴ�.

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
	bool   active = true;    // �ش� Scene �� ������ �� ���� �ǵ����ʰ�, false �� ���� Update, Render �� �̷������ �ʴ´�.
	bool   reset  = true;

	int	   display = false;

	string sceneName;	   // ���� Scene �� ��Ī
	string nextSceneName; // ���� ���� ��Ī

};

