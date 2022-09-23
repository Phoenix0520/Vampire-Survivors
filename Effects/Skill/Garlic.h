#pragma once

class Garlic : public SkillEffect
{
public:
	Garlic() {}
	~Garlic() {}

public:
	void UpdateEffect(Matrix V, Matrix P) override;
	void RenderEffect() override;
	void ResetEffect() override;

private:
	vector<Monster*> mobs;

	float rot = 0.0f;

	int spearCount = 0;
};