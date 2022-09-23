#include "framework.h"
#include "SkillEffect.h"
#include "MagicWand.h"
#include "Monster.h"
#include "Player.h"

void MagicWand::UpdateEffect(Matrix V, Matrix P)
{
	time += DELTA;

	static float tTime = 0.0f;
	static bool	 toggle = false;

	if (tTime >= 0.1f)
	{
		if (toggle)
			toggle = false;
		else
			toggle = true;

		tTime = 0.0f;
	}
	else
	{
		tTime += DELTA;
	}

	if (toggle)
	{
		texture->SetOffset(252.0f, 0.0f);
		texture->SetOffsetSize(14.0f, 25.0f);
	}
	else
	{
		texture->SetOffset(240.0f, 0.0f);
		texture->SetOffsetSize(12.0f, 25.0f);
	}

	float scale = 1.5f + 1.5f * area;
	texture->SetScale(scale, scale);
	// 스킬 범위

	float mSpeed = 500.0f + 500.0f * speed;
	// 투사체 속도

	Vector2 mPos; 
	// 몬스터 위치

	if (time >= coolTime)
	{
		if (!noMob)
			ResetEffect();
	}

	if (!IsActive())
		return;

	UINT amount = 0;

	for (int i = 0; i < MAX_MOB; i++)
	{
		if (!mobs[i]->IsActive())
		{
			amount++;

			if (amount == MAX_MOB)
				noMob = true;

			continue;
		}

		if (IsMonsterInRegion(mobs[i]))
		{
			if (!attackedID[i])
				attackedID[i] = true;
			else
				continue;

			mobs[i]->Attacked(id);
			pos2 = pl->GetPosition();
			interval = 100000.0f;

			spearCount++;

			if (spearCount >= spear)
				SetActive(false);

			break;
		}

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

	float angle = atan2((pos1.y - pos2.y), (pos1.x - pos2.x));

	pos2.x += cosf(angle) * DELTA * mSpeed;
	pos2.y += sinf(angle) * DELTA * mSpeed;

	float rot = atan2((pos1.y - pl->GetPosition().y), (pos1.x - pl->GetPosition().x));

	rot = (((-rot + (float)D3DX_PI) / (float)D3DX_PI) * 180.0f) + 90.0f;
	if (rot >= 360.0f)
		rot -= 360.0f;

	texture->SetRotation(0.0f, 180.0f, rot);
	texture->SetPosition(pos2);
	texture->Update(V, P);
}

void MagicWand::RenderEffect()
{
	if (!IsActive())
		return;

	texture->Render();
}

void MagicWand::ResetEffect()
{
	soundPlayed = false;

	spearCount = 0;

	pos2 = pl->GetPosition();
	interval = 100000.0f;
	time = 0.0f;

	ENTMANAGER->UpdateMob();
	mobs = ENTMANAGER->GetAvailMobs();
	attackedID.resize(mobs.size(), false);

	SetActive(true);
}
