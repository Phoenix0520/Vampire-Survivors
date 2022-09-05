#pragma once

class LvUpEffect : public GameObject
{
public:
	LvUpEffect();
	~LvUpEffect();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	void Random();

private:
	class Texture* texture[21];
	
	Vector2 pos[1000];
	Vector2 camPos;

	float time = 0.0f;

};