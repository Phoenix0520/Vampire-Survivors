#include "framework.h"
#include "FireEffect.h"
#include "Monster.h"
#include "Player.h"

FireEffect::FireEffect()
{
	wstring imageFile1 = L"./Images/Effects/FireEffect.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	for (int i = 0; i < 25; i++)
		texture[i] = new Texture(imageFile1, shaderFile);

	SetActive(false);
}

FireEffect::~FireEffect()
{
	for (int i = 0; i < 5; i++)
		SAFE_DELETE(texture[i]);
}

void FireEffect::Update(Matrix V, Matrix P)
{
	if (!IsActive())
		return;

	if (!ISPLAYING("Fire"))
		PLAYSOUND("Fire", sfxSize);

	Player* pl = PLAYER;
	Vector2 pPos = pl->GetPosition();
	static Vector2 pos[25];
	
	for (int i = 0; i < 25; i++)
		pos[i] = pPos;

	float interval[5] = { 13.5f, -5.7f, 9.2f, -18.0f, 4.9f };

	time -= DELTA;

	if (time < 0.0f)
	{
		SetActive(false);
	}

	if (timer >= 0.0f)
		timer -= DELTA * 30.0f;
	else
	{
		for (int i = 0; i < 25; i++)
			pos[i] = pPos;

		timer = 1.0f;
	}
	
	Vector2 oldmPos = pl->GetOldMovePos();

	if (oldmPos.x == -1.0f && oldmPos.y == -1.0f)
		rotation.z = 215.0f;
	else if (oldmPos.x == 1.0f && oldmPos.y == -1.0f)
		rotation.z = 135.0f;
	else if (oldmPos.x == -1.0f && oldmPos.y == 1.0f)
		rotation.z = 305.0f;
	else if (oldmPos.x == 1.0f && oldmPos.y == 1.0f)
		rotation.z = 45.0f;
	else if (oldmPos.x == 0.0f && oldmPos.y == -1.0f)
		rotation.z = 180.0f;
	else if (oldmPos.x == 0.0f && oldmPos.y == 1.0f)
		rotation.z = 0.0f;
	else if (oldmPos.x == 1.0f && oldmPos.y == 0.0f)
		rotation.z = 90.0f;
	else if (oldmPos.x == -1.0f && oldmPos.y == 0.0f)
		rotation.z = 270.0f;

	rotation.y = pl->GetRotation().y;

	mt19937	engine((unsigned int)std::time(NULL));
	uniform_int_distribution<> distribution(0, 4);
	auto generator = bind(distribution, engine);

	for (int i = 0; i < 25; i++)
	{
		float scale = i / 7.5f;

		if (time < 0.5f)
			scale = time * i / 3.25f;

		float angle = rotation.z - 90.0f;
		float mSpeed = 1250.0f * DELTA;

		pos[i].x += cosf((float)D3DXToRadian(angle)) * (mSpeed * i);
		pos[i].y -= sinf((float)D3DXToRadian(angle)) * (mSpeed * i);

		int random = generator();

		pos[i].y += interval[random];
		texture[i]->SetPosition(pos[i]);

		texture[i]->SetScale(scale, scale);
		
		texture[i]->UpdateColorBuffer(Color(0.75f, 0.25f, 0.1f, 0.75f), 4, 0.0f, 0.0f, 0.0f);

		texture[i]->Update(V, P);
		pos[i].y -= interval[generator()];
	}

	vector<UINT> id = pl->GetMobsInScreen();
	for (UINT i = 0; i < id.size(); i++)
	{
		Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(id[i]));

		if (IsMonsterInRegion(mob))
		{
			mob->Attacked(9);
		}
	}
}

void FireEffect::Render()
{
	if (!IsActive())
		return;

	for (int i = 0; i < 25; i++)
		texture[i]->Render();
}

void FireEffect::Reset()
{
	SetActive(true);

	Player* pl = PLAYER;

	time = 10.0f * (1 + pl->GetDuration());
	timer = 1.0f;
}

bool FireEffect::IsMonsterInRegion(Monster * mob, float advX, float advY)
{
	if (mob->GetState() == Monster::DEAD)
		return false;

	for (int i = 0; i < 25; i++)
	{
		Vector2 tPos = texture[i]->GetPosition();
		Vector2 mPos = mob->GetPosition();
		Vector2 size = texture[i]->GetTextureRealSize() + Vector2(advX, advY);

		if (mPos.x > tPos.x - size.x / 2 && mPos.x < tPos.x + size.x / 2 &&
			mPos.y > tPos.y - size.y / 2 && mPos.y < tPos.y + size.y / 2)
		{
			return true;
		}
	}

	return false;
}