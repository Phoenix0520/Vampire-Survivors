#pragma once

class ChestMenu : public GameObject
{
public:
	ChestMenu();
	~ChestMenu();

public:
	enum Rarity
	{
		NORMAL = 1,
		RARE = 3,
		EPIC = 5
	};

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

private:
	class Texture*		background	= nullptr;
	class ChestEffect*	effect		= nullptr;

	class Animation*	chest		= nullptr;
	class Animation*	chestOuter	= nullptr;

	class Button*		button		= nullptr;

	class LvUpMenu* lvUpMenu = nullptr;

	Rarity	rarity			= NORMAL;

	int		include			= 0;
	float	time			= 10.0f;
	bool	open			= false;
	bool	soundPlaying	= false;

};