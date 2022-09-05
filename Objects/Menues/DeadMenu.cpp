#include "framework.h"
#include "DeadMenu.h"
#include "Button.h"

DeadMenu::DeadMenu()
{
	wstring imageFile1 = L"./Images/InGameSceneTexture/DeadMenu/DeadBG.png";
	wstring imageFile2 = L"./Images/InGameSceneTexture/DeadMenu/GameOver.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	background = new Texture(imageFile1, shaderFile);
	gameOver   = new Texture(imageFile2, shaderFile);
	
	button = new Button();
	button->SetColor(10);
	button->SetButtonString(L"³ª°¡±â");
	button->SetButtonStringSize(50.0f);

	SetActive(false);
}

DeadMenu::~DeadMenu()
{
	SAFE_DELETE(button);
	SAFE_DELETE(gameOver);
	SAFE_DELETE(background);
}

void DeadMenu::Update(Matrix V, Matrix P)
{
	if (!IsActive())
		return;

	background->SetPosition(CAMPOS);
	background->SetScale(5.575f, 5.6225f);
	background->Update(V, P);
	background->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, 0.4f), 7, 0, 0, 0);

	gameOver->SetPosition(CAMPOS.x + 0.0f, CAMPOS.y + 192.5f);
	gameOver->SetScale(1.425f, 1.425f);
	gameOver->Update(V, P);
	
	button->SetPosition(CAMPOS.x + 0.0f, CAMPOS.y - 177.5f);
	button->Update(V, P);

	if (button->IsClick())
	{
		SCENEMANAGER->ChangeScene("GameOverScene");
		button->SetClick(false);
	}
}

void DeadMenu::Render()
{
	if (!IsActive())
		return;

	background->Render();
	gameOver->Render();
	button->Render();
}

void DeadMenu::Reset()
{
	static bool soundPlayed = false;

	if (!soundPlayed)
	{
		PLAYSOUND("GameOver", sfxSize);
		soundPlayed = true;
	}

	SetActive(true);

	button->SetHover(true);
	button->SetClick(false);
	button->SetActive(true);
}