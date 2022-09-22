#include "framework.h"
#include "ChestMenu.h"
#include "Player.h"
#include "Button.h"
#include "ChestEffect.h"
#include "LevelUpMenu.h"

ChestMenu::ChestMenu()
{
	wstring imageFile1 = L"./Images/ChestMenu.png";
	wstring imageFile2 = L"./Images/Chest/Chest.png";
	wstring imageFile3 = L"./Images/OpenChest.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	background = new Texture(imageFile1, shaderFile);

	// Chest
	{
		chest = new Animation(imageFile2, shaderFile);
		chest->SetScale(7.0f, 7.0f);
		
		// Clip 1
		{
			AnimationClip* clip = new AnimationClip(AnimationClip::loop);
			chest->AddClip(clip);

			clip->AddFrame(chest->GetTexture(), imageFile2, 0.0f, 0.0f, 30.0f, 26.0f, 3.0f);	// 1
			clip->AddFrame(chest->GetTexture(), imageFile2, 62.0f, 0.0f, 90.0f, 26.0f, 0.02f);	// 3
			clip->AddFrame(chest->GetTexture(), imageFile2, 91.0f, 0.0f, 117.0f, 26.0f, 0.1f);	// 4
			clip->AddFrame(chest->GetTexture(), imageFile2, 31.0f, 0.0f, 61.0f, 26.0f, 0.05f);	// 2
			clip->AddFrame(chest->GetTexture(), imageFile2, 118.0f, 0.0f, 154.0f, 26.0f, 0.07f);	// 5
			clip->AddFrame(chest->GetTexture(), imageFile2, 0.0f, 0.0f, 30.0f, 26.0f, 0.05f);	// 1
			clip->AddFrame(chest->GetTexture(), imageFile2, 155.0f, 0.0f, 186.0f, 26.0f, 0.07f);	// 6
			
		}

		// Clip 2
		{
			AnimationClip* clip = new AnimationClip(AnimationClip::endStay);
			chest->AddClip(clip);
				
			
			for (int i = 1; i < 8; i++)
			{
				wstring str = L"./Images/Chest/OpenChest" + to_wstring(i) + L".png";


				clip->AddFrame(chest->GetTexture(), str, 0.0f, 0.0f, 36.0f, 23.0f, 0.05f);
			}
		}
	}


	// Button
	{
		button = new Button();

		button->SetColor(11);

		button->SetButtonString(L"열기");
		button->SetButtonStringSize(50.0f);
	}

	// Effect
	{
		effect = new ChestEffect();
	}

	// Level Up Menu
	{
		lvUpMenu = new LvUpMenu();
	}

	SetActive(false);
}

ChestMenu::~ChestMenu()
{
	SAFE_DELETE(chest);
	SAFE_DELETE(background);
}

void ChestMenu::Update(Matrix V, Matrix P)
{
	if (!IsActive())
		return;

	if (button->IsClick()) // First Click : Open
	{
		open = true;
		button->SetClick(false);
	}

	background->SetPosition(CAMPOS);
	button->SetPosition(CAMPOS.x, CAMPOS.y - 292.0f);
	chest->SetPosition(CAMPOS.x - 14.0f, CAMPOS.y - 139.0f);

	if (open) // Open
	{
		if (!soundPlaying) // If not Sound Played
		{
			switch (rarity)
			{
			case ChestMenu::NORMAL:
				PLAYSOUND("Chest1", bgmSize);
				effect->SetActive(true);
				break;
			case ChestMenu::RARE:
				PLAYSOUND("Chest2", bgmSize);
				effect->SetActive(true);
				break;
			case ChestMenu::EPIC:
				PLAYSOUND("Chest3", bgmSize);
				effect->SetActive(true);
				break;
			default:
				break;
			}

			soundPlaying = true;
		}

		chest->SetPlay(1);
		chest->SetPosition(CAMPOS.x + 10.0f, CAMPOS.y - 139.0f);

		effect->SetActive(true);
		effect->Update(V, P);
	}

	background->Update(V, P);
	button->Update(V, P);
	chest->Update(V, P);
}

void ChestMenu::Render()
{
	if (!IsActive())
		return;

	background->Render();

	float time = effect->GetTime();

	if (effect->IsSkip() || time <= -6.0f) // 완전히 끝난다면
	{
		if (button->IsClick()) // 버튼을 누른다면
		{
			SetActive(false);
			effect->SetActive(false);
		}

		button->SetButtonString(L"완료");
		button->Render();
	}
	else if (open) // 완전히 끝나지 않았다면
		button->SetClick(false);

	if (time == 8.0f) // 최초 실행시
	{
		button->SetButtonString(L"열기");
		button->Render();
	}

	chest->Render();
	effect->Render();

	wstring str = L"보물 발견!";

	Vector2 pos = Vector2(0.0f, 345.0f);
	pos.x -= str.length() * 22.5f;
	pos += CAMPOS;
	CAMERA->VCToWC(pos);

	if (!open)
		DirectWrite::RenderText(str, pos, 255, 255, 255, 45.0f);
}

void ChestMenu::Reset()
{
	mt19937	engine((unsigned int)std::time(NULL));
	uniform_int_distribution<> distribution(0, 100);
	auto generator = bind(distribution, engine);
	
	float value = (float)generator();
	
	Player* pl = PLAYER;

	float luck = pl->GetLuck() * 5.0f;

	cout << "\n============= [상자 확률] =============" << endl;
	cout << "EPIC : " << 5 + luck << "%" << endl;
	cout << "RARE : " << 10 + luck << "%" << endl;
	cout << "NORMAL : " << 80 - luck * 2 << "%" << endl;

	cout << endl;

	if (value >= 95 - luck)
	{
		cout << "Value " << value << " 가 나와 Rarity : EPIC";
		rarity = EPIC;
	}
	else if (value >= 85 - luck)
	{
		cout << "Value " << value << " 가 나와 Rarity : RARE";
		rarity = RARE;
	}
	else
	{
		cout << "Value " << value << " 가 나와 Rarity : NORMAL";
		rarity = NORMAL;
	}

	cout << endl << endl;

	PLAYSOUND("FoundChest", sfxSize);

	effect->Reset();
	SetActive(true);
	chest->SetPlay(0);
	time = 10.0f;
	button->SetHover(true);
	button->SetClick(false);
	open = false;
	soundPlaying = false;
}