#include "framework.h"
#include "ScreenEffect.h"
#include "Monster.h"

ScreenEffect::ScreenEffect()
{
	wstring imageFile1 = L"./Images/Dot.png";
	wstring imageFile2 = L"./Images/Effects/ClockEffect.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	background = new Texture(imageFile1, shaderFile);

	background->SetPosition(0.0f, 0.0f);
	background->SetScale(WINWIDTH, WINHEIGHT);

	texture = new Texture(imageFile2, shaderFile);

	SetActive(false);
}

ScreenEffect::~ScreenEffect()
{
	SAFE_DELETE(background);
}

void ScreenEffect::Update(Matrix V, Matrix P)
{
	if (!IsActive())
		return;

	switch (type)
	{
	case 0:
		SetActive(false);
		break;
	case 1:
		if (time >= 1.0f)
			SetActive(false);

		time += DELTA * 2.0f;
		cout << time << endl;

		if (time <= 0.5f)
			alpha = time * 2.0f;
		else
			alpha = (1.0f - time) * 2.0f;

		for (int i = 0; i < MAX_MOB; i++)
		{
			Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));

			if (!mob->IsActive())
				continue;

			if (IsMonsterInScreen(mob))
				mob->Attacked(8);
		}

		background->UpdateColorBuffer(Color(0.25f, 0.25f, 0.0f, alpha), 7, 0, 0, 0);
		break;

	case 2:
		if (time >= 10.0f)
		{
			SetActive(false);
			for (int i = 0; i < MAX_MOB; i++)
			{
				Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));

				if (!mob->IsActive())
					continue;

				mob->SetState(Monster::MOVE);
			}
		}

		time += DELTA;

		static bool plus = true;

		if (plus)
			alpha += DELTA * 0.25f;
		else
			alpha -= DELTA * 0.25f;

		if (alpha >= 0.20f)
			plus = false;
		if (alpha <= 0.025f)
			plus = true;

		texture->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, alpha), 7, 0, 0, 0);

		static float rot = 0.0f;
		rot -= DELTA * 120.0f;
		if (rot >= -360.0f)
			rot += 360.0f;

		//texture->SetPosition(CAMPOS);
		//texture->SetRotation(0.0f, 0.0f, rot);
		//texture->SetScale(15.0f, 15.0f);
		//texture->Update(V, P);

		for (int i = 0; i < MAX_MOB; i++)
		{
			Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));

			if (!mob->IsActive())
				continue;

			if (IsMonsterInScreen(mob))
				mob->SetState(Monster::FROZEN);
		}

		background->UpdateColorBuffer(Color(0.0f, 0.1f, 1.0f, 0.25f), 4, 0, 0, 0);
		break;
	}
	
	background->SetPosition(CAMPOS);
	background->Update(V, P);
}

void ScreenEffect::Render()
{
	if (!IsActive())
		return;

	background->Render();

	//if (type == Type::ICE_EFFECT)
	//	texture->Render();
}

void ScreenEffect::Reset()
{
	SetActive(true);
	background->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, 0.0f), 4, 0, 0, 0);
	time = 0.0f;
	alpha = 0.0f;
}

bool ScreenEffect::IsMonsterInScreen(Monster* mob)
{
	if (mob->GetState() == Monster::DEAD)
		return false;

	Vector2 tPos = background->GetPosition();
	Vector2 mPos = mob->GetPosition();
	Vector2 size = background->GetTextureRealSize();

	if (mPos.x > tPos.x - size.x / 2 && mPos.x < tPos.x + size.x / 2 &&
		mPos.y > tPos.y - size.y / 2 && mPos.y < tPos.y + size.y / 2)
	{
		return true;
	}

	return false;
}
