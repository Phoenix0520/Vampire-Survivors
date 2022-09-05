#include "framework.h"
#include "Cursor.h"
#include "Player.h"

Cursor::Cursor()
{
	wstring imageFile1 = L"./Images/UI.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	// 커서
	{
		animation[0] = new Animation(imageFile1, shaderFile);

		AnimationClip* clip = new AnimationClip(AnimationClip::State::loop);

		animation[0]->AddClip(clip);
		wstring str = L"./Images/UI.png";
		{
			clip->AddFrame(animation[0]->GetTexture(), str, 118.0f, 317.0f, 134.0f, 328.0f, 0.225f);
			clip->AddFrame(animation[0]->GetTexture(), str, 181.0f, 422.0f, 197.0f, 430.0f, 0.225f);
			clip->AddFrame(animation[0]->GetTexture(), str, 16.0f , 399.0f, 32.0f , 404.0f, 0.225f);
			clip->AddFrame(animation[0]->GetTexture(), str, 198.0f, 422.0f, 214.0f, 430.0f, 0.225f);
			clip->AddFrame(animation[0]->GetTexture(), str, 110.0f, 385.0f, 126.0f, 396.0f, 0.225f);
			clip->AddFrame(animation[0]->GetTexture(), str, 165.0f, 488.0f, 181.0f, 503.0f, 0.225f);
			clip->AddFrame(animation[0]->GetTexture(), str, 146.0f, 383.0f, 162.0f, 398.0f, 0.225f);
		}

		animation[0]->SetScale(2.75f, 2.75f);
	}

	// 커서 반대쪽
	{
		animation[1] = new Animation(imageFile1, shaderFile);

		AnimationClip* clip = new AnimationClip(AnimationClip::State::loop);

		animation[1]->AddClip(clip);
		wstring str = L"./Images/UI.png";
		{
			clip->AddFrame(animation[1]->GetTexture(), str, 118.0f, 317.0f, 134.0f, 328.0f, 0.225f);
			clip->AddFrame(animation[1]->GetTexture(), str, 181.0f, 422.0f, 197.0f, 430.0f, 0.225f);
			clip->AddFrame(animation[1]->GetTexture(), str, 16.0f , 399.0f, 32.0f , 404.0f, 0.225f);
			clip->AddFrame(animation[1]->GetTexture(), str, 198.0f, 422.0f, 214.0f, 430.0f, 0.225f);
			clip->AddFrame(animation[1]->GetTexture(), str, 110.0f, 385.0f, 126.0f, 396.0f, 0.225f);
			clip->AddFrame(animation[1]->GetTexture(), str, 165.0f, 488.0f, 181.0f, 503.0f, 0.225f);
			clip->AddFrame(animation[1]->GetTexture(), str, 146.0f, 383.0f, 162.0f, 398.0f, 0.225f);
		}

		animation[1]->SetScale(2.75f, 2.75f);
		animation[1]->SetRotation(0.0f, 180.0f, 0.0f);
	}

	// 사각형 커서
	{
		wstring str = L"./Images/Cursor_Rect.png";
		texture1 = new Texture(str, shaderFile);

		str = L"./Images/Cusor_BigRect.png";
		texture2 = new Texture(str, shaderFile);
	}
}

Cursor::~Cursor()
{
	SAFE_DELETE(animation[1]);
	SAFE_DELETE(animation[0]);
	SAFE_DELETE(texture1);
	SAFE_DELETE(texture2);
}

void Cursor::Update(Matrix V, Matrix P)
{
	Vector2 pos = GetPosition();

	animation[0]->SetPosition(pos.x - gap, pos.y);
	animation[1]->SetPosition(pos.x + gap, pos.y);

	texture1->SetPosition(pos);
	texture2->SetPosition(pos);
	texture1->SetScale(scale);

	switch (type)
	{
	case 0:
		animation[0]->SetRotation(0.0f, 0.0f, 0.0f);
		animation[0]->SetScale(2.75f, 2.75f);
		animation[0]->Update(V, P);
		animation[1]->Update(V, P);
		break;
	case 1:
		texture1->Update(V, P);
		break;
	case 2:
		texture2->Update(V, P);
	case 3:
		RECT rect;
		
		rect.left	= (LONG)(CAMPOS.x - WINWIDTH / 2);
		rect.right	= (LONG)(CAMPOS.x + WINWIDTH / 2);
		rect.bottom = (LONG)(CAMPOS.y - WINHEIGHT / 2);
		rect.top	= (LONG)(CAMPOS.y + WINHEIGHT / 2);
		
		Vector2 pos = GetPosition() + Vector2(0.0f, 35.0f);

		if (rect.left <= pos.x && rect.right >= pos.x && rect.top >= pos.y && rect.bottom <= pos.y)
		{
			animation[0]->UpdateColorBuffer(Color(), 0, 0, 0, 0);
			animation[0]->SetPosition(pos.x, pos.y + 35.0f);
			animation[0]->SetRotation(0.0f, 0.0f, -90.0f);
			animation[0]->SetScale(4.25f, 4.25f);
		}
		else
		{
			animation[0]->UpdateColorBuffer(Color(0, 0, 0, 0.5f), 7, 0, 0, 0);

			Player* player = PLAYER;

			Vector2 pPos = player->GetPosition();
			Vector2 cPos = GetPosition();
			
			float x = fabsf(cPos.x - pPos.x);
			float y = fabsf(cPos.y - pPos.y);

			float itv = sqrtf(x * x + y * y);
			////////
			float angle = atan2((cPos.y - pPos.y), (cPos.x - pPos.x));

			pPos.x += cosf(angle) * (WINWIDTH / 2) * 0.875f;
			pPos.y += sinf(angle) * (WINHEIGHT / 2) * 0.875f;

			float rot = atan2((cPos.y - player->GetPosition().y), (cPos.x - player->GetPosition().x));

			rot = (((-rot + (float)D3DX_PI) / (float)D3DX_PI) * 180.0f);

			animation[0]->SetRotation(0.0f, 180.0f, rot);
			animation[0]->SetPosition(pPos);
			animation[0]->SetScale(4.25f, 4.25f);
		}

		animation[0]->Update(V, P);
		break;
	}
}

void Cursor::Render()
{
	switch (type)
	{
	case 0:
		animation[0]->Render();
		animation[1]->Render();
		break;
	case 1:
		texture1->Render();
		break;
	case 2:
		texture2->Render();
		break;
	case 3:
		animation[0]->Render();
		break;
	}
}

void Cursor::Reset()
{
	animation[0]->SetPlay(0);
	animation[1]->SetPlay(0);
}
