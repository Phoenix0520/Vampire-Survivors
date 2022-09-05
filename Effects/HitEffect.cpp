#include "framework.h"
#include "HitEffect.h"

HitEffect::HitEffect()
{
	wstring imageFile1 = L"./Images/hit1.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	texture = new Texture(imageFile1, shaderFile);	

	SetActive(false);
}

HitEffect::~HitEffect()
{
	SAFE_DELETE(texture);
}

void HitEffect::Update(Matrix V, Matrix P)
{
	if (!IsActive())
		return;

	time += DELTA * 4.0f;

	texture->SetScale(time, time);
	texture->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, time), 7, 0, 0, 0);
	texture->SetRotation(0.0f, 0.0f, 0.0f);
	texture->Update(V, P);

	if (time >= 0.75f)
	{
		SetActive(false);
		time = 0.0f;
	}
}

void HitEffect::Render()
{
	if (!IsActive())
		return;

	texture->Render();
}

void HitEffect::Reset()
{
	SetActive(true);
	texture->SetScale(0.0f, 0.0f);
	texture->SetPosition(GetPosition());
	//cout << "Hit Effect Pos : " << position.x << " , " << position.y << endl;
	time = 0.0f;
}
