#pragma once

class Whip : public SkillEffect
{
public:
	Whip() {}
	~Whip() {}

public:
	void UpdateEffect(Matrix V, Matrix P) override;
	void RenderEffect() override;
	void ResetEffect() override;

private:
	Vector2 pos = Vector2(0.0f, 0.0f);

	vector<class Monster*> mobs;

};