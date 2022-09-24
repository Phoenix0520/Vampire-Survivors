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

	Vector2 pos = Vector2(0.0f, 0.0f);

	bool plus = false;

	float rot = 0.0f;
	float ctime = 0.0f;

	int spearCount = 0;
};