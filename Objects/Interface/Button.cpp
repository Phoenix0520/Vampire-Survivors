#include "framework.h"
#include "Button.h"
#include "Cursor.h"

Button::Button()
{
	wstring imageFile1 = L"./Images/ButtonTexture.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	texture = new Texture(imageFile1, shaderFile);

	// Ä¿¼­
	{
		cursor = (Cursor*)OBJMANAGER->FindObject("Cursor");
	}
}

Button::~Button()
{
//	SAFE_DELETE(cursor);
	SAFE_DELETE(texture);
}

void Button::Update(Matrix V, Matrix P)
{   
	if (!IsActive())
		return;

	Vector2 pos2 = mouse->GetPosition();
	CAMERA->WCToVC(pos2);

	switch (color)
	{
	case 0:
		texture->SetOffset(0.0f, 0.0f);
		texture->SetOffsetSize(269.0f, 80.0f);
		break;
	case 1:
		texture->SetOffset(270.0f, 0.0f);
		texture->SetOffsetSize(135.0f, 79.0f);
		break;
	case 2:
		texture->SetOffset(405.0f, 0.0f);
		texture->SetOffsetSize(135.0f, 79.0f);
		break;
	case 3:
		texture->SetOffset(540.0f, 0.0f);
		texture->SetOffsetSize(270.0f, 83.0f);
		break;
	case 4:
		texture->SetOffset(0.0f, 81.0f);
		texture->SetOffsetSize(224.0f, 83.0f);
		break;
	case 5:
		texture->SetOffset(225.0f, 81.0f);
		texture->SetOffsetSize(178.0f, 77.0f);
		break;
	case 6:
		texture->SetOffset(404.0f, 84.0f);
		texture->SetOffsetSize(179.0f, 36.0f);
		break;
	case 7:
		texture->SetOffset(0.0f, 165.0f);
		texture->SetOffsetSize(134.0f, 78.0f);
		break;
	case 8:
		texture->SetOffset(137.0f, 166.0f);
		texture->SetOffsetSize(153.0f, 153.0f);
		break;
	case 9:
		texture->SetOffset(291.0f, 166.0f);
		texture->SetOffsetSize(200.0f, 63.0f);
		break;
	case 10:
		texture->SetOffset(494.0f, 166.0f);
		texture->SetOffsetSize(268.0f, 84.0f);
		break;
	case 11:
		texture->SetOffset(0.0f, 332.0f);
		texture->SetOffsetSize(270.0f, 84.0f);
		break;
	}
	// 290 319

	if (hover)
	{
		Vector2 pos = GetPosition();
		float val = texture->GetTextureRealSize().x;
		float gap = texture->GetScale().x;
		cursor->SetType(0);
		cursor->SetPosition(pos.x, pos.y);
		cursor->SetGap((val + 45.0f) / 2);

		if (color == 8)
		{
			cursor->SetType(2);
		}

		cursor->Update(V, P);

		if (DOWN(VK_SPACE) || DOWN(VK_RETURN))
		{
			if (!click)
				click = true;

			if (enterCallBack)
				enterCallBack(id);
		}
		
		if (DOWN('D') || DOWN(VK_RIGHT))
		{
			if (tabGroup[1])
			{
				SetHover(false);
				tabGroup[1]->SetHover(true);
			}
		}

		if (DOWN('A') || DOWN(VK_LEFT))
		{
			if (tabGroup[0])
			{
				SetHover(false);
				tabGroup[0]->SetHover(true);
			}
		}

		if (DOWN('W') || DOWN(VK_UP))
		{
			if (tabGroup[2])
			{
				SetHover(false);
				tabGroup[2]->SetHover(true);
			}
		}

		if (DOWN('S') || DOWN(VK_DOWN))
		{
			if (tabGroup[3])
			{
				SetHover(false);
				
				tabGroup[3]->SetHover(true);
			}
		}
	}

	
	texture->SetPosition(position);

	texture->SetRotation(rotation);
	texture->SetScale(scale);
	texture->Update(V, P);

	return;
}
             
void Button::Render()
{
	if (!IsActive())
		return;

	texture->Render();

	if (hover)
		cursor->Render();
	
	float x = -(script.size() * 25.0f * 0.5f);
	float y = texture->GetTextureRealSize().y * 0.25f;
	float font = 25.0f;

	switch ((int)scriptSize)
	{
	case 20:
		font = 20.0f;
		x += 7.5f;
		y += 5.0f;
		break;
	case 27:
		font = 25.0f;
		x -= 17.5f;
		y += 27.5f;
		break;
	case 30:
		font = 35.0f;
		x = -(script.size() * font * 0.5f);
		y += 10.0f;
		break;
	case 35:
		font = 35.0f;
		x = -(script.size() * font * 0.5f);
		y += 7.5f;
		break;
	case 50:
		font = 47.5f;
		x = -(script.size() * font * 0.5f);
		y += 15.0f;

		break;
	}

	Vector2 pos = Vector2(GetPosition().x + x, GetPosition().y + y);
	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(script, pos, 255, 255, 255, font);
}

void Button::Reset()
{
	click = false;
	hover = false;
}

void Button::SetHover(bool val)
{
	hover = val;
	cursor->Reset();
}

void Button::UpdateColorBuffer(Color color, int path, float a, float b, float c)
{
	if (path == 0)
		texture->UpdateColorBuffer(Color(), path, a, b, c);

	texture->UpdateColorBuffer(color, path, a, b, c);
}

bool Button::IsPointInRegion(Vector2 position)
{
	Vector2 size = Vector2(texture->GetTextureRealSize());
	Vector2 min = Vector2(GetPosition().x - size.x * 0.5f, GetPosition().y - size.y * 0.5f);
	Vector2 max = Vector2(GetPosition().x + size.x * 0.5f, GetPosition().y + size.y * 0.5f);

	if ((position.x >= min.x && position.x <= max.x) && (position.y >= min.y && position.y <= max.y))
	{
		return true;
	}
	else
		return false;
}