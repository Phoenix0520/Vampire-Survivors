#pragma once

class Scene
{
public:
	Scene() {};
	~Scene() {};

public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void ChangeScene() = 0;
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

