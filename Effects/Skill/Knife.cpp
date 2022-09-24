#include "framework.h"
#include "SkillHeader.h"
#include "Knife.h"

void Knife::UpdateEffect(Matrix V, Matrix P)
{
	static float tTime = 0.0f;
	static bool toggle = false;

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
		texture->SetOffset(271.0f, 0.0f);
		texture->SetOffsetSize(6.0f, 35.0f);
	}
	else
	{
		texture->SetOffset(265.0f, 0.0f);
		texture->SetOffsetSize(6.0f, 30.0f);
	}

	time += DELTA;

	Vector3 rot = pl->GetRotation();
	static Vector2 oldmPos = mPos;
	static float oldAngle = 90.0f;

	if (time >= coolTime)
	{
		ResetEffect();
	}

	if (!IsActive())
		return;

	if (mPos.x == 0 && mPos.y == 1)
		rot.z = 0.0f;
	else if (mPos.x == 1 && mPos.y == 1)
		rot.z = 45.0f;
	else if (mPos.x == 1 && mPos.y == 0)
		rot.z = 90.0f;
	else if (mPos.x == 1 && mPos.y == -1)
		rot.z = 135.0f;
	else if (mPos.x == 0 && mPos.y == -1)
		rot.z = 180.0f;
	else if (mPos.x == -1 && mPos.y == -1)
		rot.z = 225.0f;
	else if (mPos.x == -1 && mPos.y == 0)
		rot.z = 270.0f;
	else if (mPos.x == -1 && mPos.y == 1)
		rot.z = 315.0f;

	if (mPos.x == 0 && mPos.y == 0)
	{
		rot.z = oldAngle;
		mPos = pl->GetOldMovePos();
		texture->SetRotation(0.0f, 180.0f, rot.z);
	}
	else
	{
		oldAngle = rot.z;
		oldmPos = mPos;
		texture->SetRotation(0.0f, 180.0f, rot.z);
	}

	// 방향

	if (mPos.x != 0.0f && mPos.y != 0.0f)
	{
		pos.x += (mPos.x * (15.0f * (1.0f + speed))) / sqrtf(2.0f);
		pos.y += (mPos.y * (15.0f * (1.0f + speed))) / sqrtf(2.0f);
	}
	else
	{
		pos.x += mPos.x * (15.0f * (1.0f + speed));
		pos.y += mPos.y * (15.0f * (1.0f + speed));
	}
	
	texture->SetPosition(pos);
	// 속도 및 이동

	float scale = 2.25f + 2.25f * area;

	texture->SetScale(scale, scale);

	// 스킬 범위
	
	for (int i = 0; i < MAX_MOB; i++)
	{
		if (IsMonsterInRegion(mobs[i], 15.0f, 15.0f))
		{
			mobs[i]->Attacked(id);
			spearCount++;

			if (spearCount >= spear)
			{
				spearCount = 0;
				SetActive(false);
				return;
			}
		}
	}

	texture->Update(V, P);
}

void Knife::RenderEffect()
{
	if (!IsActive())
		return;

	texture->Render();
}

void Knife::ResetEffect()
{
	soundPlayed = false;

	texture->SetPosition(pl->GetPosition());
	time = 0.0f;

	mPos = pl->GetMovePos();
	pos = texture->GetPosition();

	ENTMANAGER->UpdateMob();
	mobs = ENTMANAGER->GetAvailMobs();
	attackedID.clear();
	attackedID.resize(mobs.size());

	SetActive(true);
}
