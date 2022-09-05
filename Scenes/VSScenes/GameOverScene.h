#pragma once

class GameOverScene : public Scene
{
public:
	GameOverScene();
	~GameOverScene();

public:
	void Update() override;
	void Render() override;
	void ChangeScene() override;

	void SetProperty(int min, int sec, int level, int gold, int kill);
	void ReadTextFile(int row, Vector2& offset, Vector2& offSize);

private:
	class Texture*		background	= nullptr;
	class Texture*		texture		= nullptr;
	class Button*		button		= nullptr;

	class Player*		player		= nullptr;
	class EquipList*	list		= nullptr;

	int finalMin		= 0;
	int finalSec		= 0;
	int finalLevel		= 0;
	int finalGold		= 0;
	int finalKillCount	= 0;
};