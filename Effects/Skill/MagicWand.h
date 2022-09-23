#pragma once

class MagicWand : public SkillEffect
{
public:
	MagicWand() {}
	~MagicWand() {}

public:
	void UpdateEffect(Matrix V, Matrix P) override;
	void RenderEffect() override;
	void ResetEffect() override;

private:
	Vector2 pos1 = Vector2(0.0f, 0.0f);
	Vector2 pos2 = Vector2(0.0f, 0.0f);

	vector<class Monster*> mobs;

	float interval = 0.0f;

	int spearCount = 0;

	bool noMob = false;
};