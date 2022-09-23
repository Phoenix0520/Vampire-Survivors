#include "framework.h"
#include "SkillHeader.h"
#include "Bible.h"

void Bible::UpdateEffect(Matrix V, Matrix P)
{
	texture->SetOffset(307.0f, 0.0f);
	texture->SetOffsetSize(10.0f, 15.0f);
	
	time += DELTA;
	
	if (time >= coolTime)
	{
		ResetEffect();
	}
	
	if (time >= duration)
	{
		SetActive(false);
	}
	
	if (!IsActive())
		return;
	
	float x1 = cosf((float)D3DXToRadian(angle)) * (125.0f * (1 + (area / 2)));
	float y1 = sinf((float)D3DXToRadian(angle)) * (125.0f * (1 + (area / 2)));

	pos = pl->GetPosition();

	Vector2 start = Vector2(x1, y1);
	pos -= start;
	texture->SetPosition(pos);
	pos += start;
	// 위치 설정
	
	float mSpeed = (360.0f / duration) * (1 + speed);
	angle += mSpeed * DELTA;
	// 이동속도
	
	float scale = 2.5f + 2.5f * area;
	
	if (time <= 0.5f)
		scale *= (time + 0.5f);
	else if (time >= duration - 0.5f)
		scale *= (-time + duration + 0.5f);
	else
		scale = 2.5f + 2.5f * area;
	
	texture->SetScale(scale, scale);
	// 크기 설정
	
	texture->SetRotation(0.0f, 0.0f, 0.0f);
	// 각도 변화 없음
	
	texture->Update(V, P);
	
	for (int i = 0; i < MAX_MOB; i++)
	{
		if (IsMonsterInRegion(mobs[i]))
		{
			if (!attackedID[i])
				attackedID[i] = true;
			else
				continue;

			mobs[i]->Attacked(id);
		}
	}
}

void Bible::RenderEffect()
{
	if (!IsActive())
		return;

	texture->Render();
}

void Bible::ResetEffect()
{
	time = 0.0f;
	angle = 135.0f;

	ENTMANAGER->UpdateMob();
	mobs = ENTMANAGER->GetAvailMobs();
	attackedID.resize(mobs.size());

	SetActive(true);
}
