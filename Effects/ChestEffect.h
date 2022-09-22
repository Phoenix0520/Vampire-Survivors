#pragma once

class ChestEffect : public GameObject
{
public:
	ChestEffect();
	~ChestEffect();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	float GetTime() { return time; }
	bool IsSkip() { return skip; }

	float GetGold() { return gold; }

	void ReadTextFile(int row, Vector2& offset, Vector2& offsetSize);

	void GoldRender();
	void ItemRoullet();
	void Explain();
	void ConfirmItem();

private:
	class Texture* texture	= nullptr;
	class Texture* item		= nullptr;
	class Texture* itemBG	= nullptr;
	class Texture* itemBox	= nullptr;
	class Texture* explain	= nullptr;
	class Texture* vfx		= nullptr;

	class LvUpMenu* lvUpMenu = nullptr;

	Vector2 pos;

	UINT	icon[7] = { 0, 1, 2, 3, 4, 5, 6 };
	UINT	type = 0;

	bool	skip = false;
	bool	confirm = false;

	float	scale = 0.0f;
	float	gold = 0.0f;
	float	value = 0.0f;
	float	ftime = 0.0f;
	float	time = 0.0f;
	float	rtime = 0.0f;
	
};