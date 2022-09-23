#pragma once

class Knife : public SkillEffect
{
public:
	Knife() {}
	~Knife() {}

public:
	void UpdateEffect(Matrix V, Matrix P) override;
	void RenderEffect() override;
	void ResetEffect() override;

private:
	Vector2 mPos = Vector2(0.0f, 0.0f);
	Vector2 pos = Vector2(0.0f, 0.0f);

	vector<Monster*> mobs;

	int spearCount = 0;

};