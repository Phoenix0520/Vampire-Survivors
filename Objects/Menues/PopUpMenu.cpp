#include "framework.h"
#include "PopUpMenu.h"
#include "Button.h"
#include "CheckBox.h"
#include "SlideBox.h"

PopUpMenu::PopUpMenu()
{
	wstring imageFile1 = L"./Images/PopUpMenuAll.png";
	wstring imageFile2 = L"./Images/PopUpMenuSubAll.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	// Windows
	{
		window[0] = new Texture(imageFile1, shaderFile);
		window[1] = new Texture(imageFile2, shaderFile);
	}

	// Pause Menu
	{
		imageFile2 = L"./Images/rect.png";
		rect = new Texture(imageFile1, shaderFile);
		rect->SetScale(WINWIDTH, WINHEIGHT);
		rect->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, 0.5f), 4, 0, 0, 0);
	}
}

PopUpMenu::~PopUpMenu()
{
	SAFE_DELETE(rect);
	SAFE_DELETE(window[1]);
	SAFE_DELETE(window[0]);
}

void PopUpMenu::Update(Matrix V, Matrix P)
{
	if (!IsActive())
		return;

	switch (id)
	{
	case 0:
	{
		break;
	}
	case 4:
	{
		window[0]->SetOffset(720.0f, 0.0f);
		window[0]->SetOffset(720.0f, 620.0f);
		window[0]->SetPosition(CAMPOS.x, CAMPOS.y + 80.0f);
		window[0]->Update(V, P);

		for (int i = 0; i < 3; i++)
			button[i]->Update(V, P);
	
		slideBox[0]->Update(V, P);
		slideBox[1]->Update(V, P);
		
		checkBox[0]->Update(V, P);
		checkBox[1]->Update(V, P);
		checkBox[2]->Update(V, P);

		break;
	}
	}
}

void PopUpMenu::Render()
{
	if (!IsActive())
		return;

	switch (id)
	{
	case 0:
	{
		break;
	}
	case 4:
	{
		rect->Render();

		for (int i = 0; i < 3; i++)
			button[i]->Render();

		window[0]->Render();

		slideBox[0]->Render();
		slideBox[1]->Render();

		checkBox[0]->Render();
		checkBox[1]->Render();
		checkBox[2]->Render();

		wstring str = L"사운드";
		float x = -(str.length() * 35.0f * 0.5f);
		float y = 90.0f;
		Vector2 pos = Vector2(x - 265.0f, y + 240.0f);
		pos += CAMPOS;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"음악";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 280.0f, y + 145.0f);
		pos += CAMPOS;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"시각효과 (vfx)";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 210.0f, y + 17.5f);
		pos += CAMPOS;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"조이스틱 표시";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 210.0f, y - 90.0f);
		pos += CAMPOS;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"데미지 표시";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 227.5f, y - 200.0f);
		pos += CAMPOS;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		break;
	}

	default:
	{
		break;
	}

	}
}

void PopUpMenu::Reset()
{
}
