#include "framework.h"
#include "BloodEffect.h"
#include "Player.h"
#include "Monster.h"

BloodEffect::BloodEffect()
{
	wstring imageFile1 = L"./Images/Effects/BloodEffect.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	texture = new Texture(imageFile1, shaderFile);
	texture->SetPosition(0.0f, 0.0f);

	for (int i = 0; i < 20; i++)
	{
		gPos[i] = Vector2(0.0f, 0.0f);
		gravity[i] = 0.0f;
	}
}

BloodEffect::~BloodEffect()
{
	SAFE_DELETE(texture);
}

void BloodEffect::Update(Matrix V, Matrix P)
{
	if (!active)
		return;

	timer += DELTA;

	if (timer < wtime)
		return;

	if (plus)
	{
		if (time < 0.225f)
			time += DELTA * 0.75f;
		else
			plus = false;
	}
	else
	{
		if (time > -0.75f)
		{
			time -= DELTA * 0.5f;
		}
	}

	float val = 9.0f * time;
	if (val < 0.0f)
		val = 0.0f;

	texture->SetScale(val, val);
	texture->Update(V, P);
}

void BloodEffect::Render()
{
	if (!active)
		return;

	if (timer < wtime)
		return;

	//for (int i = 0; i < 20; i++)
		//gPos[i] = GetPosition();

	//float roll = TIMEMANAGER->GetRunning() * 800.0f;
	//SetRotation(0.0f, 0.0f, roll);

	float angle[20] = { 0.65f, -0.47f, -0.87f, -0.28f, 0.73f, 0.21f, -0.59f, 0.32f, -0.32f, 0.57f, -0.67f, 0.85f, -0.85f, -0.95f, 0.37f, 0.68f, -0.53f, 0.72f };

	for (int i = 0; i < 20; i++)
	{
		if (angle[i] < 0)
			gPos[i].x -= cosf(angle[i]) * 3.5f;
		else
			gPos[i].x += cosf(angle[i]) * 3.5f;

		gPos[i].y += sinf(0.875f) * 9.0f - gravity[i];

		gravity[i] += 0.5f;

		Player* pl = PLAYER;

		texture->SetPosition(gPos[i] + pl->GetPosition());
		texture->Update(VM, PM);
		texture->Render();
	}
}

void BloodEffect::Reset()
{
	texture->SetScale(1.0f, 1.0f);

	time = 0.0f;
	val = 0.0f;

	for (int i = 0; i < 20; i++)
	{
		gPos[i] = GetPosition();
		gravity[i] = 0.0f;
	}

	plus = true;
}
