#include "framework.h"
#include "SlideBox.h"
#include "Cursor.h"
#include "Rect.h"

SlideBox::SlideBox()
{
	wstring imageFile1 = L"./Images/SlideBox.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	texture = new Texture(imageFile1, shaderFile);
	texture->SetScale(2.825f, 2.825f);

	shaderFile = L"./Shader/HLSL/Color.hlsl";
	rect = new Rect(shaderFile);
	rect->UpdateBlock(Color(0.0f, 0.0f, 0.0f, 1.0f));
	rect->SetScale(177.5f, 5.5f);

	cursor = (Cursor*)OBJMANAGER->FindObject("Cursor");
	cursor->SetGap(57.5f);
}

SlideBox::~SlideBox()
{
	SAFE_DELETE(rect);
	SAFE_DELETE(texture);
}

void SlideBox::Update(Matrix V, Matrix P)
{
	if (!active)
		return;

	Vector2 pos = GetPosition();

	if (hover)
	{
		cursor->SetGap(57.5f);
		cursor->SetPosition(texture->GetPosition());
		cursor->Update(V, P);

		if (DOWN('A') || DOWN(VK_LEFT))
		{
			if (percent > 0.0f)
				percent -= 10.0f;
		}
		else if (DOWN('D') || DOWN(VK_RIGHT))
		{
			if (percent < 100.0f)
				percent += 10.0f;
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

	float val = percent * 1.455f;
	rect->SetPosition(pos);
	texture->SetPosition(pos.x + val - 85.5f, pos.y);
	
	rect->Update(V, P);
	texture->Update(V, P);
}

void SlideBox::Render()
{
	if (!active)
		return;
	
	rect->Render();
	texture->Render();

	if (hover)
	{
		cursor->Render();
	}
}

void SlideBox::Reset()
{
	percent = 10.0f;
}
