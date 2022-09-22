#pragma once

class LvUpMenu : public GameObject
{
public:
	LvUpMenu();
	~LvUpMenu();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	void Random();
	void SetProperty();
	
	bool IsSelect() { return select; }
	void SetSelect(bool val) { select = val; }

	void Upgrade(int type);

	vector<wstring> GetExplains() { return explains; }
	vector<string> GetAddType() { return addType; }
	vector<float> GetAddValue() { return addValue; }

private:
	void ReadTextFile(int row, Vector2& offset, Vector2& offsetSize);
	void IncreasingItemValue(int id, int level);

private:
	class Texture*		window	= nullptr;
	class Texture*		button	= nullptr;
	class Texture*		item	= nullptr;
	class Cursor*		cursor	= nullptr;
	class EquipList*	list	= nullptr;
	class LvUpEffect*	effect	= nullptr;
	class InGameUI*		UI		= nullptr;

	vector<wstring> explains;
	vector<string> addType;
	vector<float> addValue;
	vector<int> random;

	int way[4];
	int wayCount = 3;
	int hover = 0;

	int* equipSkillList = nullptr;
	int* equipItemList = nullptr;

	float time = 0.0f;

	bool select = false;
	bool maxEnchant = false;
};