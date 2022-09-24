#pragma once

class DamageCount
{
public:
	DamageCount() {}
	~DamageCount() {}

public:
	void Render();
	void Reset(Vector2 pos, float val);

private:
	Vector2 pos = Vector2(0.0f, 0.0f);

	int damage = 0;

	float time = 0.0f;
	float size = 0.0f;

	bool active = false;

};