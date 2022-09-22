#pragma once

class Brazier : public GameObject
{
public:
	Brazier();
	~Brazier();

public:
	enum Include
	{
		Coin = 1,
		CoinBag,
		BigCoinBag,
		Chicken,
		Rosary,
		Nduja,
		Clock,
		Vaccum,
		Clover,
	};


public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	void SetInclude(UINT val) { include = (Include)val; }
	void SetInclude(Include val) { include = val; }
	UINT GetInclude() { return (UINT)include; }

	void SetHP(float val) { hp = val; }
	float GetHP() { return hp; }

	void Attacked(float damage);

private:
	class Texture* texture = nullptr;

	Include	include = Coin;

	float hp = 10.0f;
	float time = 0.5f;

	bool underAttack = false;
};