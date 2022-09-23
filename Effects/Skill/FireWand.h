#pragma once

class FireWand : public SkillEffect
{
public:
	FireWand() {}
	~FireWand() {}

public:
	void UpdateEffect(Matrix V, Matrix P) override;
	void RenderEffect() override;
	void ResetEffect() override;

private:
	vector<Monster*> mobs;

	Vector2 mPos = Vector2(0.0f, 0.0f);
	Vector2 pos = Vector2(0.0f, 0.0f);
	Vector2 pos1 = Vector2(0.0f, 0.0f);
	Vector2 pos2 = Vector2(0.0f, 0.0f);

	float angle = 0.0f;

	int spearCount = 0;
};