#include "framework.h"
#include "CheckBox.h"
#include "Cursor.h"

CheckBox::CheckBox()
{
	wstring imageFile1 = L"./Images/CheckBox.png";
	wstring imageFile2 = L"./Images/Mark.png";

	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	// 체크박스
	{
		texture = new Texture(imageFile1, shaderFile);
		texture->SetScale(2.825f, 2.825f);
	}

	// 커서
	{
		cursor = (Cursor*)OBJMANAGER->FindObject("Cursor");
	}
}

CheckBox::~CheckBox()
{
	SAFE_DELETE(texture);
}

void CheckBox::Update(Matrix V, Matrix P)
{
	if (!active)
		return;

	Vector2 pos = mouse->GetPosition();
	CAMERA->WCToVC(pos);
	cursor->SetPosition(GetPosition());
	cursor->SetGap(57.5f);

	if (hover)
	{
		cursor->Update(V, P);

		if ((DOWN(VK_SPACE) || DOWN(VK_RETURN)))
		{
			if (check)
				check = false;
			else
				check = true;
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

	if (IsPointInRegion(pos) && mouse->Down(0))
	{
		if (check)
			check = false;
		else
			check = true;
	}

	texture->SetPosition(position.x, position.y);
	texture->SetOffset(0.0f, 0.0f);
	texture->SetOffsetSize(24.0f, 24.0f);
	texture->Update(V, P);
}

void CheckBox::Render()
{
	if (!active)
		return;

	Matrix V = CAMERA->GetViewMatrix();
	Matrix P = CAMERA->GetProjMatrix();

	texture->Render();

	texture->SetOffsetSize(16.0f, 16.0f);

	if (check)
		texture->SetOffset(25.0f, 0.0f);
	else
		texture->SetOffset(42.0f, 0.0f);
	
	texture->Update(V, P);
	texture->Render();

	if (hover)
	{
		cursor->Render();
	}
}

void CheckBox::Reset()
{
	hover = false;
	check = false;
}

bool CheckBox::IsPointInRegion(Vector2 position)
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