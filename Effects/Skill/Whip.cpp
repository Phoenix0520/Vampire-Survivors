#include "framework.h"
#include "SkillHeader.h"
#include "Whip.h"

void Whip::UpdateEffect(Matrix V, Matrix P)
{
	texture->SetOffset(0.0f, 0.0f);
	texture->SetOffsetSize(240.0f, 53.0f);

	time += DELTA;

	pos = pl->GetPosition();

	if (time >= coolTime)
	{
		ResetEffect();
	}

	float alpha = time / coolTime + 0.5f;

	texture->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, alpha * 1.5f), 7, 0, 0, 0);

	float scale = 1.0f + 1.0f * area;

	if (time <= 0.15f)
		texture->SetScale(time * 10.0f * scale, time * 10.0f * scale);
	else if (time > 0.15f && time <= 0.2f)
		texture->SetScale((time - 0.15f) * 1.5f, (time - 0.15f) * 1.5f);

	if (texture->GetScale().x < 0.1f)
		texture->SetScale(0.0f, 0.0f);

	for (UINT i = 0; i < mobs.size(); i++)
	{
		Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster40");

		if (IsMonsterInRegion(mobs[i]))
		{
			if (!attackedID[i])
				attackedID[i] = true;
			else
				continue;

			PLAYSOUND("Hit", sfxSize);
			mobs[i]->Attacked(id);
		}
	}

	texture->Update(V, P);
}

void Whip::RenderEffect()
{
	texture->Render();
}

void Whip::ResetEffect()
{
	soundPlayed = false;

	texture->SetRotation(GetRotation());

	pos = pl->GetPosition();

	if (GetRotation().y == 0.0f)
		texture->SetPosition(pos.x + 140.0f, pos.y + 2.5f);
	else
		texture->SetPosition(pos.x - 140.0f, pos.y + 2.5f);

	time = 0.0f;

	ENTMANAGER->UpdateMob();
	mobs = ENTMANAGER->GetAvailMobs();

	attackedID.resize(mobs.size(), false);
}