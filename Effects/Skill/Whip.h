#pragma once

class Whip : public SkillEffect
{
public:
	Whip();
	~Whip();

public:
	void UpdateEffect(Matrix V, Matrix P);
	void RenderEffect();
	void ResetEffect();

private:
	Vector2 pos = Vector2(0.0f, 0.0f);
	
	float time = 0.0f;

	vector<bool> attackedID;
	vector<class Monster*> mobs;



};