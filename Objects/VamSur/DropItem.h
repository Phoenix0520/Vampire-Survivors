#pragma once

#define MAX_ITEM 50

class DropItem : public GameObject
{
public:
	 DropItem();
	~DropItem();

public:
	enum Type
	{
		NONE = -1,
		EXP,
		Coin,
		CoinBag,
		BigCoinBag,
		Chicken,
		Rosary,
		Nduja,
		Clock,
		Vaccum,
		Clover,
		Chest
	};

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	UINT GetType()			{ return (UINT)type; }
	void SetType(Type val)	{ type = val; }

	int  GetID()		{ return id; }
	void SetID(int val) { id = val; }

	bool GetUsed()			{ return used;}
	void SetUsed(bool val)	{ used = val;}

	void UsingItem();
	bool IsPlayerInRegion(float adv = 0.0f);
	bool IsPlayerInRegionEx(float adv = 0.0f);

private:
	class Texture*		texture		= nullptr;
	class Animation*	animation	= nullptr;
	class Player*		player		= nullptr;
	class Cursor*		arrow		= nullptr;

	class Collider*		playerCenterDot = nullptr;

	Type type = NONE;

	int id = 0;

	float time = 0.0f;
	float angle = 0.0f;
	float expVal = 0.0f;

	Vector2 pos1;
	Vector2 pos2;

	bool used = false;
	bool gettering = false;
	bool inhailing = false;
};