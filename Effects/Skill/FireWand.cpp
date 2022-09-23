#include "framework.h"
#include "SkillHeader.h"
#include "FireWand.h"

void FireWand::UpdateEffect(Matrix V, Matrix P)
{
	static bool toggle = false;
	static float tTime = 0.0f;
	
	if (toggle)
	{
		texture->SetOffset(331.0f, 0.0f);
		texture->SetOffsetSize(14.0f, 29.0f);
	}
	else
	{
		texture->SetOffset(345.0f, 0.0f);
		texture->SetOffsetSize(14.0f, 29.0f);
	}
	
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
	
	
	time += DELTA;
	
	if (time >= coolTime)
	{
		ResetEffect();
	}
	
	if (!IsActive())
		return;
	
	pos.x += cosf((float)D3DXToRadian(angle)) * DELTA * (150.0f * (1 + speed));
	pos.y += sinf((float)D3DXToRadian(angle)) * DELTA * (150.0f * (1 + speed));
	
	texture->SetPosition(pos);
	// 이동 및 속도
	
	texture->SetRotation(0.0f, 0.0f, angle - 90.0f);
	// 회전
	
	float scale = 2.5f + (2.5f * area);
	
	texture->SetScale(scale, scale);
	// 크기
	
	for (UINT i = 0; i < MAX_MOB; i++)
	{
		if (IsMonsterInRegion(mobs[i]))
		{
			if (!attackedID[i])
				attackedID[i] = true;
			else
				continue;
	
			mobs[i]->Attacked(id);
			SetActive(false);
			return;
		}
	}
	
	texture->Update(V, P);
}

void FireWand::RenderEffect()
{
	if (!IsActive())
		return;

	texture->Render();
}

void FireWand::ResetEffect()
{
	mt19937	engine((unsigned int)std::time(NULL));
	uniform_int_distribution<> distribution(0, 360);
	auto generator = bind(distribution, engine);

	time = 0.0f;
	pos = pl->GetPosition();
	angle = (float)generator();

	ENTMANAGER->UpdateMob();
	mobs = ENTMANAGER->GetAvailMobs();
	attackedID.resize(mobs.size());

	SetActive(true);
}
