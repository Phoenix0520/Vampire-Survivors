#include "framework.h"
#include "LvUpEffect.h"

LvUpEffect::LvUpEffect()
{
	wstring imageFile1 = L"./Images/InGameSceneTexture/LevelUpMenu/LevelUpEffect.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	float val = 0.0f;
	for (int i = 0; i < 21; i++)
	{
		texture[i] = new Texture(imageFile1, shaderFile);
		texture[i]->SetOffsetSize(9.0f, 12.0f);

		texture[i]->SetOffset(val, 0.0f);
		val += 9.0f;
		if (val == 27.0f)
			val = 0.0f;
	}

	Reset();
}

LvUpEffect::~LvUpEffect()
{
	for (int i = 0; i < 21; i++)
		SAFE_DELETE(texture[i]);
}

void LvUpEffect::Update(Matrix V, Matrix P)
{
	//float val1 = 150.0f * DELTA;
	//float val2 = 100.0f * DELTA;
	//Vector2 val3 = Vector2(val2 / 112.5f, val2 / 112.5f);

	//for (int i = 0; i < 21; i++)
	//{
	//	Vector2 pos = texture[i]->GetPosition();
	//	
	//	texture[i]->SetPosition(pos.x, pos.y - val1);
	//	texture[i]->SetRotation(0.0f, 0.0f, texture[i]->GetRotation().z + val2);
	//	
	//	if (texture[i]->GetScale().x > 0.0f)
	//		texture[i]->SetScale(texture[i]->GetScale() - val3);
	//	
	//	texture[i]->Update(V, P);

	//	if (pos.y < -(WINHEIGHT / 2 + CAMPOS.y) + 25.0f)
	//		Reset();
	//}



	
}

void LvUpEffect::Render()
{
	//for (int i = 0; i < 21; i++)
	//{
	//	float height = WINHEIGHT / 2 - 80.0f;

	//	if (texture[i]->GetPosition().y < height)
	//		texture[i]->Render();
	//}
	
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			texture[j]->SetPosition(pos[i * j] + CAMPOS);

			float val = 2.5f * ((pos[i * j].y + WINHEIGHT / 2) / WINHEIGHT);
			texture[j]->SetScale(val, val);

			texture[j]->Update(VM, PM);
			texture[j]->Render();
		}
	}
}

void LvUpEffect::Reset()
{
	Random();

}

void LvUpEffect::Random()
{
	mt19937	engine((unsigned int)std::time(NULL));
	uniform_int_distribution<> distribution1(-(int)(WINHEIGHT / 2), (int)(WINHEIGHT / 2));
	auto generator2 = bind(distribution1, engine);

	uniform_int_distribution<> distribution2(-(int)(WINWIDTH / 2), (int)(WINWIDTH / 2));
	auto generator1 = bind(distribution2, engine);

	for (int i = 0; i < 1000; i++)
	{
		pos[i] = Vector2((float)generator1(), (float)generator2());
	}
}