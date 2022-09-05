#include "framework.h"
#include "ChestEffect.h"

ChestEffect::ChestEffect()
{
	wstring imageFile1 = L"./Images/Chest/OpenEffect1.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	texture[0] = new Texture(imageFile1, shaderFile);

	SetActive(false);
}

ChestEffect::~ChestEffect()
{
	SAFE_DELETE(texture[0]);
}

void ChestEffect::Update(Matrix V, Matrix P)
{
	if (!IsActive())
		return;

	Vector2 pos = CAMPOS;
	pos.y -= 143.0f;

	switch (rarity)
	{
	case ChestEffect::NORMAL:
		texture[0]->UpdateColorBuffer(Color(25, 25, 255, 0.85f), 7, 0, 0, 0);
		break;
	case ChestEffect::RARE:

		break;
	case ChestEffect::EPIC:

		break;
	default:
		break;
	}

	if (time < 1.0f)
		time += DELTA;

	texture[0]->SetScale(7.0f, 7.0f * time);

	Vector2 size = texture[0]->GetTextureRealSize();

	texture[0]->SetPosition(pos.x, pos.y + (size.y / 2));
	texture[0]->Update(V, P);
}

void ChestEffect::Render()
{
	if (!IsActive())
		return;

	texture[0]->Render();
}

void ChestEffect::Reset()
{
	time = 0.0f;
	SetActive(true);

	cout << "Chest Effect Reset!" << endl;
}

void ChestEffect::Init()
{
	wstring imageFile1 = L"./Images/Chest/OpenEffect1.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	texture[0] = new Texture(imageFile1, shaderFile);
}