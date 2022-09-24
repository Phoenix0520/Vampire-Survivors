#include "framework.h"
#include "SkillHeader.h"
#include "Garlic.h"

void Garlic::UpdateEffect(Matrix V, Matrix P)
{
	texture->SetOffset(360.0f, 0.0f);
	texture->SetOffsetSize(157.0f, 157.0f);
	
	time += DELTA;

	if (time >= coolTime)
	{
		ResetEffect();
	}

	if (plus)
		ctime += DELTA * 0.25f;
	else
		ctime -= DELTA * 0.25f;

	if (ctime >= 0.4f)
		plus = false;
	if (ctime <= 0.1f)
		plus = true;

	float alpha = ctime;

	texture->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, alpha), 7, 0, 0, 0);

	rot += DELTA * 120.0f;
	if (rot >= 360.0f)
		rot -= 360.0f;

	texture->SetRotation(0.0f, 0.0f, rot);

	float scale = 1.0f + 1.0f * area;

	texture->SetScale(scale, scale);

	pos = pl->GetPosition();
	pos.y -= 12.5f;

	texture->SetPosition(pos);

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
			return;
		}
	}
}

void Garlic::RenderEffect()
{
	texture->Render();
}

void Garlic::ResetEffect()
{
	time = 0.0f;

	ENTMANAGER->UpdateMob();
	mobs = ENTMANAGER->GetAvailMobs();
	attackedID.clear();
	attackedID.resize(mobs.size());

	SetActive(true);
}
