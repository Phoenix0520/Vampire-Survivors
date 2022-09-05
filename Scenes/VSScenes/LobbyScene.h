#pragma once

class LobbyScene : public Scene
{
public:
	LobbyScene();
	~LobbyScene();

public:
	void Update() override;
	void Render() override;
	void ChangeScene() override;

	void UpdateBG(Matrix V, Matrix P);
	void UpdateIL(Matrix V, Matrix P);
	void UpdateBT(Matrix V, Matrix P);

	void RenderBG();
	void RenderIL();
	void RenderBT();

	void OptionMenuUpdate();
	void OptionMenuRender();

	void StaffMenuUpdate();
	void StaffMenuRender();

	void CollectionMenuUpdate();
	void CollectionMenuRender();

	void SelectCharMenuUpdate();
	void SelectCharMenuRender();

	void GetBackToMainMenu();

	void RenderCoin();

	void OnButtonClicked(int buttonID);

private:
	class Texture* background	= nullptr;
	class Texture* title		= nullptr;
	class Texture* texture[3];
	class Texture* menuBar		= nullptr;

	class Texture* window		= nullptr;
	class Texture* sDialogue	= nullptr;

	class Rect* rect			= nullptr;

	class Button* button[11];
	class ListCtrl* list		= nullptr;

	class CheckBox* checkBox[30];
	class SlideBox* slideBox[2];

	class EquipList* skillList	= nullptr;

	int step	= 0;
	int memory	= 0;

	float time	= 0.0f;
	float timer = 1.5f;
	float btime = 0.0f;

	bool enter			= false;
	bool boop			= true;
	bool click			= false;
	bool charSelected	= false;

	bool optionMenu		= false;
	bool staffMenu		= false;
	bool collectionMenu = false;
	bool selectCharMenu = false;
};