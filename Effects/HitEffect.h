#pragma once

class HitEffect : public GameObject
{
public:
	HitEffect();
	~HitEffect();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

private:
	class Texture* texture = nullptr;

	float time = 0.0f;

	bool plus = true;
};
