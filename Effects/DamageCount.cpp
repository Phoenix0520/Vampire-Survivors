#include "framework.h"
#include "DamageCount.h"

void DamageCount::Render()
{
	if (!active)
		return;

	wstring str = to_wstring(damage);

	Vector2 temp = pos;

	time += DELTA;

	float value = 70.0f;

	if (time < 0.5f)
		size = time * value;
	else
		size = -(time - 0.5f) * value;

	if (size <= 0.0f)
	{
		active = false;
		return;
	}

	pos.x -= str.length() * size * 0.5f;
	pos.y += size * 0.5f;

	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(str, pos, 255, 255, 255, size);

	pos = temp;
}

void DamageCount::Reset(Vector2 pos, float val)
{
	this->pos = pos + Vector2(20.0f, 20.0f);
	damage = (int)val;
	time = 0.0f;
	size = 0.0f;

	active = true;
}
