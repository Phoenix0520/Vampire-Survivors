#pragma once

class BloodEffect : public GameObject
{
public:
	BloodEffect();
	~BloodEffect();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	void SetId(int val) { angleID = val; }
	void SetWait(float val) { wtime = val; }
	bool IsPlayed() { if (texture->GetScale() == Vector2(0.0f, 0.0f)) return true; else return false; }

private:
	class Texture* texture = nullptr;

	int	angleID = 0;
	float time = 0.0f;
	float timer = 0.0f;
	float wtime = 0.0f;
	float val = 0.0f;
	float gravity[20];
	Vector2 gPos[20];

	bool plus = true;
};
