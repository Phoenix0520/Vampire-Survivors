#include "framework.h"
#include "SkillHeader.h"
#include "Axe.h"

void Axe::UpdateEffect(Matrix V, Matrix P)
{
	texture->SetOffset(277.0f, 0.0f);
	texture->SetOffsetSize(16.0f, 18.0f);
	
	time += DELTA;
	
	if (time >= coolTime)
	{
		ResetEffect();
	}
	
	if (!IsActive())
		return;
	
	if (angle == 0.0f)
		pos.x += 0.0f;
	else if (angle > 0.0f)
		pos.x += cosf(angle) * DELTA * 350.0f;
	else
		pos.x -= cosf(angle) * DELTA * 350.0f;
	
	pos.y += sinf(0.975f) * 1000.0f * DELTA - gravity;
	
	gravity += 0.45f + 0.45f * speed;
	
	texture->SetPosition(pos);
	
	// 속도 및 이동
	
	rot = (time * 0.5f) * 540.0f;
	
	texture->SetRotation(0.0f, 180.0f, rot);
	
	// 회전 설정
	
	float scale = 2.75f + 2.75f * area;
	
	texture->SetScale(scale, scale);
	// 크기 설정
	
	for (int i = 0; i < MAX_MOB; i++)
	{
		if (IsMonsterInRegion(mobs[i]))
		{
			if (!attackedID[i])
				attackedID[i] = true;
			else
				continue;
	
			mobs[i]->Attacked(id);
			spearCount++;
	
			if (spearCount >= spear)
			{
				SetActive(false);
				break;
			}
			
		}
	}
	
	texture->Update(V, P);
}

void Axe::RenderEffect()
{
	if (!IsActive())
		return;

	texture->Render();
}

void Axe::ResetEffect()
{
	pos = pl->GetPosition();
	gravity = 0.0f;
	angle = 0.0f;
	time = 0.0f;

	spearCount = 0;

	ENTMANAGER->UpdateMob();
	mobs = ENTMANAGER->GetAvailMobs();
	attackedID.resize(mobs.size());

	SetActive(true);
}
