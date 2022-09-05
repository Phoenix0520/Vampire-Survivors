#pragma once

class FireEffect : public GameObject
{
public:
	FireEffect();
	~FireEffect();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	bool IsMonsterInRegion(class Monster* mob, float advX = 0.0f, float advY = 0.0f);

private:
	class Texture* texture[25];
	class Animation* animation = nullptr;

	float time = 0.0f;
	float timer = 1.0f;
};