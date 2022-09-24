#include "framework.h"
#include "SkillHeader.h"
#include "Cross.h"

void Cross::UpdateEffect(Matrix V, Matrix P)
{
	texture->SetOffset(293.0f, 0.0f);
	texture->SetOffsetSize(14.0f, 14.0f);
	
	time += DELTA;

	if (time >= coolTime)
	{
		ResetEffect();
	}
	
	if (!IsActive())
		return;
	// 투사체 속도
	
	if (time <= 0.35f)
	{
		pos2.x += cosf(angle) * DELTA * (-time * 2.0f + 1.5f) * mSpeed;
		pos2.y += sinf(angle) * DELTA * (-time * 2.0f + 1.5f) * mSpeed;
	}
	else
	{
		pos2.x -= cosf(angle) * DELTA * (time + 0.75f) * mSpeed;
		pos2.y -= sinf(angle) * DELTA * (time + 0.75f) * mSpeed;
	}

	mSpeed = 400.0f + 400.0f * (time + 0.75f) * speed;
	
	float scale = 2.0f + 2.0f * area;
	texture->SetScale(scale, scale);
	// 스킬 범위
	texture->SetPosition(pos2);
	// 이동 및 속도
	
	rot += 720.0f * (time + 0.75f) * DELTA;
	texture->SetRotation(0.0f, 180.0f, rot);
	// 회전
	
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

			if (time < 0.2f)
				time = 0.2f;
		}
	}
}

void Cross::RenderEffect()
{
	if (!IsActive())
		return;

	texture->Render();
}

void Cross::ResetEffect()
{
	time = 0.0f;
	mPos = Vector2(0.0f, 0.0f);
	pos1 = Vector2(0.0f, 0.0f);
	pos2 = pl->GetPosition();

	interval = 100000.0f;

	ENTMANAGER->UpdateMob();
	mobs = ENTMANAGER->GetAvailMobs();
	attackedID.clear();
	attackedID.resize(mobs.size());

	for (int i = 0; i < MAX_MOB; i++)
	{
		if (!mobs[i]->IsActive())
			continue;

		mPos = mobs[i]->GetPosition();

		float x = fabsf(mPos.x - pos2.x);
		float y = fabsf(mPos.y - pos2.y);

		float itv = sqrtf(x * x + y * y);

		if (itv < interval)
		{
			pos1 = mPos;
			interval = itv;
		}
	}

	angle = atan2(pos1.y - pos2.y, pos1.x - pos2.x);

	SetActive(true);
}
