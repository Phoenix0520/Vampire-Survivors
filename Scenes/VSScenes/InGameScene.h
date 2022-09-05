#pragma once

class InGameScene : public Scene
{
public:
	InGameScene();
	~InGameScene();

public:
	void Update() override;
	void Render() override;
	void ChangeScene() override;

	void PauseMenuUpdate();
	void PauseMenuRender();

	void LevelUpMenuUpdate();
	void LevelUpMenuRender();

	void CreateBrazier();

	void OnButtonClicked(int buttonID);
	void ShowGUI();

private:
	class Texture* background = nullptr;
	class Texture* texture[2];
	class Texture* levelBar = nullptr;
	class Texture* levelVal = nullptr;
	class Texture* equipedSkill = nullptr;
	
	class Texture* rect = nullptr;
	class Texture* option = nullptr;
	class Texture* levelUp = nullptr;
	class Button*  button[3];

	class SlideBox* slideBox[2];
	class CheckBox* checkBox[3];

	class Player* player = nullptr;
	class Monster* mob[50];
	class LvUpMenu* lvUpMenu = nullptr;
	class InGameUI* UI = nullptr;

	class DropItem* item[50];
	class Brazier* brazier[50];

	class ChestMenu* chestMenu = nullptr;
	class DeadMenu* deadMenu = nullptr;

	// System

	float ctime = 0.0f;
	float time = 0.0f;
	float mtime[50];

	float val[12];

	bool plus = true;

	bool pauseMenu = false;
	bool optionMenu = false;
	bool levelMenu = false;

	// InGame
	int kill = 0;
	int gold = 0;

};