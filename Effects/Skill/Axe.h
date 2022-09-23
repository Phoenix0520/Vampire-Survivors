#pragma once

class Axe : public SkillEffect
{
public:
	Axe() {}
	~Axe() {}

public:
	void UpdateEffect(Matrix V, Matrix P) override;
	void RenderEffect() override;
	void ResetEffect() override;

private:
	vector<Monster*> mobs;

	Vector2 mPos = Vector2(0.0f, 0.0f);
	Vector2 pos = Vector2(0.0f, 0.0f);

	float rot		= 0.0f;
	float angle		= 0.0f;
	float gravity	= 0.0f;

	int spearCount	 = 0;

};