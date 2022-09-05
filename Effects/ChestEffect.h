#pragma once

class ChestEffect : public GameObject
{
public:
	ChestEffect();
	~ChestEffect();

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

	void Init();

	void SetRarity(UINT val) { rarity = (Rarity)val; }
	void SetRarity(Rarity val) { rarity = val; }

private:
	class Texture* texture[5];

	Rarity	rarity = NORMAL;

	float	time = 0.0f;
	
};