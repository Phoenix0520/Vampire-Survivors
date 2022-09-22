#include "framework.h"
#include "InGameScene.h"
#include "ObjectList.h"
#include "LevelUpMenu.h"
#include "ChestMenu.h"
#include "DeadMenu.h"
#include "GameOverScene.h"

#define B_RESUME	0
#define B_OPTION	1
#define B_EXIT		2

#define VISIBLE_GUI true

InGameScene::InGameScene()
{
	wstring imageFile1 = L"./Images/Background/BackGround.png";
	wstring imageFile2 = L"./Images/BackGround/BG (1).png";
	wstring imageFile3 = L"./Images/BackGround/BG (2).png";
	wstring imageFile4 = L"./Images/BackGround/BG (3).png";

	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	background = new Texture(imageFile3, shaderFile);
	background->SetScale(2.0f, 2.0f);
	background->SetPosition(-background->GetTextureRealSize());

	// Pause Menu
	{
		imageFile3 = L"./Images/rect.png";
		rect = new Texture(imageFile3, shaderFile);
		rect->SetScale(WINWIDTH, WINHEIGHT);
		rect->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, 0.5f), 4, 0, 0, 0);

		imageFile4 = L"./Images/InGameSceneTexture/PauseMenu/PauseMenuOption.png";
		option = new Texture(imageFile4, shaderFile);
	}

	function<void(int)> callback;
	callback = bind(&InGameScene::OnButtonClicked, this, placeholders::_1);

	for (int i = 0; i < 3; i++)
	{
		Button* bt = (Button*)OBJMANAGER->FindObject("Button" + to_string(i + 11));

		switch (i)
		{
		case 0:
			bt->SetColor(3);
			bt->SetButtonString(L"계속하기");
			bt->SetButtonStringSize(35.0f);
			bt->SetID(11);
			break;
		case 1:
			bt->SetColor(3);
			bt->SetButtonString(L"옵션");
			bt->SetButtonStringSize(35.0f);
			bt->SetID(12);
			break;
		case 2:
			bt->SetColor(9);
			bt->SetButtonString(L"종료");
			bt->SetButtonStringSize(30.0f);
			bt->SetID(13);
			break;
		}
		bt->SetCallback(callback);
		bt->SetActive(false);
		button[i] = bt;
	}

	// SlideBox
	{
		slideBox[0] = (SlideBox*)OBJMANAGER->FindObject("SlideBox0");
		slideBox[1] = (SlideBox*)OBJMANAGER->FindObject("SlideBox1");

		//slideBox[0]->SetID(0);

		slideBox[0]->SetPosition(160.0f, 250.0f);
		slideBox[1]->SetPosition(160.0f, 145.0f);
	}

	// CheckBox
	{
		for (int i = 0; i < 3; i++)
			checkBox[i] = (CheckBox*)OBJMANAGER->FindObject("CheckBox" + to_string(i));
	}

	// Tag Groub Setting
	{
		button[B_RESUME]->SetLeftTabGroup(button[B_OPTION]);
		button[B_RESUME]->SetTopTabGroup(nullptr);

		button[B_OPTION]->SetRightTabGroup(button[B_RESUME]);

		button[B_EXIT]->SetRightTabGroup(button[B_RESUME]);
	}

	{
		lvUpMenu = new LvUpMenu();
		OBJMANAGER->AddObject("LvUpMenu", lvUpMenu);

		UI = (InGameUI*)OBJMANAGER->FindObject("InGameUI");
	}

	ZeroMemory(val, sizeof(float) * 12);

	for (int i = 0; i < MAX_MOB; i++)
	{
		mob[i] = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));
		mob[i]->SetPosition(100000.0f, 0.0f);
		//item[1] = new DropItem();
		//item[1]->SetPosition(0.0f, 150.0f);
		//item[1]->SetType(DropItem::Nduja);
	}



	for (int i = 0; i < MAX_ITEM; i++)
	{
		item[i] = (DropItem*)OBJMANAGER->FindObject("DropItem" + to_string(i));
		item[i]->SetPosition(0.0f, 0.0f);

		brazier[i] = (Brazier*)OBJMANAGER->FindObject("Brazier" + to_string(i));

		//item[1] = new DropItem();
		//item[1]->SetPosition(0.0f, 150.0f);
		//item[1]->SetType(DropItem::Nduja);
	}

	chestMenu = new ChestMenu();
	OBJMANAGER->AddObject("ChestMenu", chestMenu);
	deadMenu = new DeadMenu();

	SetActive(false);

	sceneName = "InGameScene";
}

InGameScene::~InGameScene()
{
	SAFE_DELETE(background);
	SAFE_DELETE(texture[0]);
	SAFE_DELETE(texture[1]);
}

void InGameScene::Update()
{
	CAMERA->Update();
	Matrix V = CAMERA->GetViewMatrix();
	Matrix P = CAMERA->GetProjMatrix();

	if (GetFocus() != MAIN->GetWindowHandler())
		return;

	if (player->GetHp() <= 0.0f)
	{
		static float dTime = 1.0f;
		dTime -= DELTA;

		if (dTime < 0.0f)
		{
			player->SetFinalMin(UI->GetMin());
			player->SetFinalSec(UI->GetSec());
			
			deadMenu->Reset();
		}
	}

	// Level Up Menu
	{
		if (levelMenu)
		{
			LevelUpMenuUpdate();
			UI->SetActive(false);
			UI->Update(V, P);
			return;
		}
		else
			UI->SetActive(true);

		if (player->GetLvUpRemain() > 0)
		{
			lvUpMenu->Reset();

			//LvUpEffect* lvUpEffect = (LvUpEffect*)OBJMANAGER->FindObject("LvUpEffect");
			//lvUpEffect->Reset();
			levelMenu = true;
			return;
		}
	}


	if (chestMenu->IsActive())
	{
		chestMenu->Update(V, P);
		return;
	}

	if (!ISPLAYING("BGM"))
		PLAYSOUND("BGM", bgmSize);

	// Esc Menu
	{
		if (DOWN(VK_ESCAPE))
		{
			if (pauseMenu)
			{
				button[B_EXIT]->SetActive(false);
				button[B_EXIT]->SetHover(false);

				checkBox[2]->SetActive(false);
				checkBox[1]->SetActive(false);
				checkBox[0]->SetActive(false);
				slideBox[1]->SetActive(false);
				slideBox[0]->SetActive(false);

				checkBox[2]->SetHover(false);
				checkBox[1]->SetHover(false);
				checkBox[0]->SetHover(false);
				slideBox[1]->SetHover(false);
				slideBox[0]->SetHover(false);

				optionMenu = false;
				pauseMenu = false;
			}
			else
			{
				button[B_RESUME]->SetHover(true);
				button[B_RESUME]->SetActive(true);
				button[B_OPTION]->SetActive(true);

				checkBox[2]->SetActive(true);
				checkBox[1]->SetActive(true);
				checkBox[0]->SetActive(true);
				slideBox[1]->SetActive(true);
				slideBox[0]->SetActive(true);


				pauseMenu = true;
			}
		}

		if (pauseMenu)
		{
			PauseMenuUpdate();
			return;
		}
	}

	// 배경
	{
		background->Update(V, P);
	}

	// 플레이어
	{
		player->SetActive(true);
		player->Update(V, P);
	}

	// 아이템
	{
		for (int i = 0; i < MAX_ITEM; i++)
		{
			item[i]->Update(V, P);
		}
	}

	// 몬스터
	{
		CreateMonster();

		for (int i = 0; i < MAX_MOB; i++)
		{
			mob[i]->Update(V, P);
		}
	}

	// 화로
	{
		CreateBrazier();

		for (int i = 0; i < MAX_ITEM; i++)
		{
			brazier[i]->Update(V, P);
		}
	}

	// System
	{
		if (!deadMenu->IsActive())
			UI->Update(V, P);
	}

	// Dead Menu
	{
		deadMenu->Update(V, P);
	}

	// Entity Manager
	{
		ENTMANAGER->Update(V, P);
	}
}


void InGameScene::Render()
{
	BEGINDRAW;
	// 배경
	{
		background->Render();

		for (int i = 0; i < 11; i++)
		{
			for (int j = 0; j < 11; j++)
			{
				background->SetPosition(background->GetTextureRealSize().x * (i - 5), background->GetTextureRealSize().y * (j - 5));
				background->Update(VM, PM);
				background->Render();
			}
		}
	}
	// 플레이어
	{
		player->Render();
	}

	// 아이템
	{
		for (int i = 0; i < MAX_ITEM; i++)
		{
			item[i]->Render();
		}
	}

	// 몬스터
	{
		for (int i = 0; i < MAX_MOB; i++)
		{
			mob[i]->Render();
		}
	}

	// 화로
	{
		for (int i = 0; i < MAX_ITEM; i++)
		{
			brazier[i]->Render();
		}
	}

	// System
	{
		UI->Render();
	}

	// Pause Menu
	{
		if (pauseMenu)
			PauseMenuRender();
	}

	// Level Up Menu
	{
		if (levelMenu)
			LevelUpMenuRender();
	}

	// Dead Menu
	{
		deadMenu->Render();
	}

	// Chest Menu
	{
		chestMenu->Render();
	}

	// EntityManager
	{
		ENTMANAGER->Render();
	}

	ENDDRAW;

	ShowGUI();
}

void InGameScene::ChangeScene()
{
	SetActive(true);

	slideBox[0]->SetTopTabGroup(nullptr);
	slideBox[0]->SetBottomTabGroup(slideBox[1]);

	slideBox[1]->SetTopTabGroup(slideBox[0]);
	slideBox[1]->SetBottomTabGroup(checkBox[0]);

	checkBox[0]->SetTopTabGroup(slideBox[1]);
	checkBox[0]->SetBottomTabGroup(checkBox[1]);

	checkBox[1]->SetTopTabGroup(checkBox[0]);
	checkBox[1]->SetBottomTabGroup(checkBox[2]);

	checkBox[2]->SetTopTabGroup(checkBox[1]);
	checkBox[2]->SetBottomTabGroup(button[B_RESUME]);

	slideBox[0]->SetPosition(180.0f, 320.0f);
	slideBox[1]->SetPosition(180.0f, 200.0f);

	float y = 80.0f;
	float gap = -120.0f;

	for (int i = 0; i < 3; i++)
	{
		checkBox[i]->SetPosition(95.0f, y + gap * i);
	}

	time = 0.0f;

	mt19937 engine((unsigned int)GetTickCount());
	uniform_int_distribution<> distribution(-500, 500);
	auto generator = bind(distribution, engine);
	
	for (int i = 0; i < MAX_MOB; i++)
	{
		float x = 0.0f;
		float y = 0.0f;
	
		Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));
	
		x = (float)generator();
		y = (float)generator();
	
		mob->Reset();
		mob->SetMoving(true);
		mob->SetMoveSpeed(30.0f);
		mob->SetIntersect(true);
		mob->SetDamage(20.0f);
		mob->SetPosition(x, y);
		mob->SetActive(true);
	}

	for (int i = 0; i < MAX_ITEM; i++)
	{
		DropItem* item = (DropItem*)OBJMANAGER->FindObject("DropItem" + to_string(i));
		item->SetActive(false);
	}

	player = (Player*)OBJMANAGER->FindObject("Player");
	player->SetActive(true);

	for (int i = 0; i < 12; i++)
	{
		val[i] = player->SetGUI(i);
	}

	player->Reset();
	CAMERA->SetObject(player);

	UI->Reset();

	for (int i = 0; i < 8; i++)
	{
		SkillEffect* se = (SkillEffect*)OBJMANAGER->FindObject("SkillEffect" + to_string(i));
		se->Reset();
	}

	background->UpdateColorBuffer(Color(), 0, 0, 0, 0);
}

void InGameScene::PauseMenuUpdate()
{
	{
		rect->SetPosition(CAMPOS);
		rect->Update(VM, PM);
	}

	{
		EquipList* list = (EquipList*)OBJMANAGER->FindObject("EquipList");
		list->SetRenderType(1);
		list->Update(VM, PM);
	}

	for (int i = 0; i < 3; i++)
	{
		Vector2 pos = Vector2(0.0f, 0.0f);

		switch (i)
		{
		case 0:
			pos = Vector2(167.0f, -390.0f);
			break;
		case 1:
		case 2:
			pos = Vector2(-167.0f, -390.0f);
			break;
		}

		button[i]->SetPosition(pos + CAMPOS);
		button[i]->Update(VM, PM);
	}

	if (optionMenu)
	{
		button[B_RESUME]->SetLeftTabGroup(button[B_EXIT]);
		button[B_RESUME]->SetTopTabGroup(checkBox[2]);

		option->SetPosition(CAMPOS.x, CAMPOS.y + 80.0f);
		option->Update(VM, PM);

		static Vector2 pos0 = slideBox[0]->GetPosition();
		static Vector2 pos1 = slideBox[1]->GetPosition();
		static Vector2 pos2 = checkBox[0]->GetPosition();
		static Vector2 pos3 = checkBox[1]->GetPosition();
		static Vector2 pos4 = checkBox[2]->GetPosition();

		slideBox[0]->SetPosition(pos0 + CAMPOS);
		slideBox[1]->SetPosition(pos1 + CAMPOS);

		checkBox[0]->SetPosition(pos2 + CAMPOS);
		checkBox[1]->SetPosition(pos3 + CAMPOS);
		checkBox[2]->SetPosition(pos4 + CAMPOS);

		slideBox[0]->Update(VM, PM);
		slideBox[1]->Update(VM, PM);

		checkBox[0]->Update(VM, PM);
		checkBox[1]->Update(VM, PM);
		checkBox[2]->Update(VM, PM);
	}
	else
	{
		button[B_RESUME]->SetTopTabGroup(nullptr);
		button[B_RESUME]->SetLeftTabGroup(button[B_OPTION]);
	}
}

void InGameScene::PauseMenuRender()
{
	rect->Render();

	for (int i = 0; i < 3; i++)
		button[i]->Render();

	// list
	{
		EquipList* list = (EquipList*)OBJMANAGER->FindObject("EquipList");
		list->Render();
	}

	if (optionMenu)
	{
		option->Render();

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
		pos = Vector2(x - 280.0f, y + 130.0f);
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
	}

}

void InGameScene::LevelUpMenuUpdate()
{
	lvUpMenu->Update(VM, PM);

	if (lvUpMenu->IsSelect())
	{
		player->SetLvUpRemain(player->GetLvUpRemain() - 1);
		player->LevelUp();
		levelMenu = false;
	}

	// list
	{
		EquipList* list = (EquipList*)OBJMANAGER->FindObject("EquipList");
		list->SetRenderType(1);
		list->Update(VM, PM);
	}
}

void InGameScene::LevelUpMenuRender()
{
	lvUpMenu->Render();

	// list
	{
		EquipList* list = (EquipList*)OBJMANAGER->FindObject("EquipList");
		list->Render();
	}
}

void InGameScene::CreateBrazier()
{
	static float ctime = 0.0f;

	if (ctime <= 0.0f)
	{
		UINT index = ENTMANAGER->GetAddableBrazIndex();
		string str = "Brazier" + to_string(index);

		Brazier* obj = (Brazier*)OBJMANAGER->FindObject(str);
		obj->Reset();

		ENTMANAGER->UpdateBrazier();

		ctime = 20.0f;
	}
	else
		ctime -= DELTA;
}

void InGameScene::CreateMonster()
{
	return;

	mt19937 engine((unsigned int)GetTickCount());
	uniform_int_distribution<> distribution(-1000, 1000);
	auto generator = bind(distribution, engine);
	
	static float ratio = 1.2f;

	if (rtime >= ratio)
	{
		for (UINT i = 0; i < MAX_MOB; i++)
		{
			Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));

			if (mob->IsActive())
				continue;
			else
			{
				float x = (float)generator();
				float y = (float)generator();

				mob->Reset();
				mob->SetMoving(true);
				mob->SetMoveSpeed(30.0f);
				mob->SetIntersect(true);
				mob->SetDamage(20.0f);
				mob->SetPosition(x, y);
				mob->SetActive(true);

				if (i == 30)
				{
					mob->SetHp(150.0f);
					mob->SetScale(5.0f, 5.0f);
				}

				rtime = 0.0f;
				ratio *= 0.975f;
				return;
			}
		}
	}
	else
		rtime += DELTA;

}

void InGameScene::OnButtonClicked(int buttonID)
{
	for (int i = 0; i < 3; i++)
	{
		button[i]->Reset();
	}

	switch (buttonID - 11)
	{
	case B_RESUME:
		pauseMenu = false;
		optionMenu = false;

		button[B_RESUME]->SetActive(false);
		button[B_OPTION]->SetActive(false);
		button[B_EXIT]->SetActive(false);
		break;

	case B_OPTION:
		optionMenu = true;

		button[B_OPTION]->SetActive(false);
		button[B_EXIT]->SetActive(true);
		button[B_RESUME]->SetHover(true);
		break;

	case B_EXIT:
		pauseMenu = false;
		optionMenu = false;
		button[B_RESUME]->SetActive(false);
		button[B_OPTION]->SetActive(false);
		button[B_EXIT]->SetActive(false);
		SCENEMANAGER->ChangeScene("LobbyScene");
		break;
	}
}

void InGameScene::ShowGUI()
{
	if (!VISIBLE_GUI)
		return;

	ImGui::GetIO().NavActive = false;
	ImGui::GetIO().WantCaptureMouse = true;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	mt19937	engine((unsigned int)std::time(NULL));

	EquipList* skills = (EquipList*)OBJMANAGER->FindObject("EquipList");
	ListCtrl* lc = (ListCtrl*)OBJMANAGER->FindObject("ListCtrl");

	int* sl = player->GetEquipedSkillList();
	int amount = 0;
	for (int i = 0; i < 8; i++)
	{
		if (sl[i] != -1)
			amount++;
	}

	ImGui::Begin("Debug Tools");
	{
		if (ImGui::BeginTabBar(""))
		{
			// ImGui
			if (ImGui::BeginTabItem("Skills"))
			{
				if (ImGui::BeginListBox("Skills", ImVec2(245.0f, 55.0f)))
				{
					if (ImGui::Button("Whip"))
					{
						int id = S_WHIP;

						if (amount != 6)
							if (!skills->GetSkillEquip(id))
							{
								if (skills->GetSkillLevel(id) <= 0)
									skills->UpdateSkillLevel(id, 1);

								skills->UpdateSkillEquip(id, true);
								player->SetEquipedSkill(amount, id);

								lc->UpdateItemCollect(id * 2, true);
							}
					}
					ImGui::SameLine();
					if (ImGui::Button("MAGIC Wand"))
					{
						int id = S_MAGIC;

						if (amount != 6)
							if (!skills->GetSkillEquip(id))
							{
								if (skills->GetSkillLevel(id) <= 0)
									skills->UpdateSkillLevel(id, 1);

								skills->UpdateSkillEquip(id, true);
								player->SetEquipedSkill(amount, id);

								lc->UpdateItemCollect(id * 2, true);
							}
					}
					ImGui::SameLine();
					if (ImGui::Button("SWORD"))
					{
						int id = S_SWORD;

						if (amount != 6)
							if (!skills->GetSkillEquip(id))
							{
								if (skills->GetSkillLevel(id) <= 0)
									skills->UpdateSkillLevel(id, 1);

								skills->UpdateSkillEquip(id, true);
								player->SetEquipedSkill(amount, id);

								lc->UpdateItemCollect(id * 2, true);
							}
					}
					ImGui::SameLine();
					if (ImGui::Button("AXE"))
					{
						int id = S_AXE;

						if (amount != 6)
							if (!skills->GetSkillEquip(id))
							{
								if (skills->GetSkillLevel(id) <= 0)
									skills->UpdateSkillLevel(id, 1);

								skills->UpdateSkillEquip(id, true);
								player->SetEquipedSkill(amount, id);

								lc->UpdateItemCollect(id * 2, true);
							}
					}

					if (ImGui::Button("CROSS"))
					{
						int id = S_CROSS;

						if (amount != 6)
							if (!skills->GetSkillEquip(id))
							{
								if (skills->GetSkillLevel(id) <= 0)
									skills->UpdateSkillLevel(id, 1);

								skills->UpdateSkillEquip(id, true);
								player->SetEquipedSkill(amount, id);

								lc->UpdateItemCollect(id * 2, true);
							}
					}
					ImGui::SameLine();
					if (ImGui::Button("BIBLE"))
					{
						int id = S_BIBLE;

						if (amount != 6)
							if (!skills->GetSkillEquip(id))
							{
								if (skills->GetSkillLevel(id) <= 0)
									skills->UpdateSkillLevel(id, 1);

								skills->UpdateSkillEquip(id, true);
								player->SetEquipedSkill(amount, id);

								lc->UpdateItemCollect(id * 2, true);
							}
					}
					ImGui::SameLine();
					if (ImGui::Button("FIRE Wand"))
					{
						int id = S_FIRE;

						if (amount != 6)
							if (!skills->GetSkillEquip(id))
							{
								if (skills->GetSkillLevel(id) <= 0)
									skills->UpdateSkillLevel(id, 1);

								skills->UpdateSkillEquip(id, true);
								player->SetEquipedSkill(amount, id);

								lc->UpdateItemCollect(id * 2, true);
							}
					}
					ImGui::SameLine();
					if (ImGui::Button("GARLIC"))
					{
						int id = S_GARLIC;

						if (amount != 6)
							if (!skills->GetSkillEquip(id))
							{
								if (skills->GetSkillLevel(id) <= 0)
									skills->UpdateSkillLevel(id, 1);

								skills->UpdateSkillEquip(id, true);
								player->SetEquipedSkill(amount, id);

								lc->UpdateItemCollect(id * 2, true);
							}
					}

					ImGui::EndListBox();

					if (ImGui::Button("Reset A", ImVec2(40.0f, 25.0f)))
					{
						cout << "Equipment Reset!" << endl;
						for (int i = 0; i < 12; i++)
						{
							for (int i = 0; i < 6; i++)
							{
								player->SetEquipedSkill(i, -1);
								player->SetEquipedItem(i, -1);
							}

							for (int i = 0; i < 8; i++)
							{
								skills->UpdateSkillEquip(i, false);
								skills->UpdateSkillLevel(i, 0);
							}
						}
					}

					ImGui::SameLine();

					if (ImGui::Button("All MaxLv", ImVec2(75.0f, 25.0f)))
					{
						cout << "Equipment All Max Level!" << endl;
						for (int i = 0; i < 8; i++)
							skills->UpdateSkillLevel(i, 8);
						for (int i = 0; i < 12; i++)
							skills->UpdateItemLevel(i, 5);
					}
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Status"))
			{
				static int lvVal = 0;

				if (ImGui::InputInt("Value", &lvVal))
				{

				}
				ImGui::SameLine();
				if (ImGui::Button("Level Up"))
				{
					player->AddExp(lvVal * 100.0f);
				}

				if (ImGui::Button("Suiside"))
				{
					player->SetHp(0.0f);
				}

				ImGui::Separator();

				// Max HP, Recovery 
				if (ImGui::SliderFloat("Max HP", &val[0], 0.0f, 500.0f, "%.f"))
				{
					player->SetMaxHp(val[0]);
				}

				if (ImGui::SliderFloat("Recovery", &val[1], 0.0f, 10.0f))
				{
					player->SetRecovery(val[1]);
				}

				// Defence, MoveSpeed
				if (ImGui::SliderFloat("Defence", &val[2], 0.0f, 10.0f, "%.f"))
				{
					player->SetDef(val[2]);
				}

				if (ImGui::SliderFloat("MoveSpeed", &val[3], 0.0f, 500.0f, "%.f"))
				{
					player->SetMoveSpeed(val[3] / 100.0f);
				}

				// Power, SkillSpeed
				if (ImGui::SliderFloat("Power", &val[4], 0.0f, 500.0f, "%.f"))
				{
					player->SetPower(val[4] / 100.0f);
				}

				if (ImGui::SliderFloat("SkillSpeed", &val[5], 0.0f, 500.0f, "%.f"))
				{
					player->SetSkillSpeed(val[5] / 100.0f);
				}

				// Duration, Area
				if (ImGui::SliderFloat("Duration", &val[6], 0.0f, 500.0f, "%.f"))
				{
					player->SetDuration(val[6] / 100.0f);
				}

				if (ImGui::SliderFloat("Area", &val[7], 0.0f, 500.0f, "%.f"))
				{
					player->SetArea(val[7] / 100.0f);
				}

				// CoolTime, Amount
				if (ImGui::SliderFloat("CoolTime", &val[8], 0.0f, 500.0f, "%.f"))
				{
					player->SetCoolDown(val[8] / 100.0f);
				}

				if (ImGui::SliderFloat("Amount", &val[9], 0.0f, 5.0f, "%.f"))
				{
					player->SetAmount(val[9]);
				}

				// Luck, Growth
				if (ImGui::SliderFloat("Luck", &val[10], 0.0f, 1000.0f, "%.f"))
				{
					player->SetLuck(val[10] / 100.0f);
				}

				if (ImGui::SliderFloat("Growth", &val[11], 0.0f, 500.0f, "%.f"))
				{
					player->SetGrowth(val[11] / 100.0f);
				}
				if (ImGui::Button("Reset B", ImVec2(40.0f, 25.0f)))
				{
					player->ResetAbility();

					for (int i = 0; i < 12; i++)
					{
						val[i] = player->SetGUI(i);
					}
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Drop Item"))
			{
				static const char* items[] =
				{
					"NONE", "EXP", "Coin", "CoinBag", "BigCoinBag", "Chicken",
					"Rosary", "Nduja", "Clock", "Vaccum", "Clover", "Chest"
				};

				static int combo;

				ImGui::Combo("Type", &combo, items, ARRAYSIZE(items));

				static UINT amount = 0;

				for (int i = 0; i < MAX_ITEM - 1; i++)
				{
					if (item[amount]->IsActive())
						amount++;
					else
						break;
				}

				item[amount]->SetType((DropItem::Type)(combo - 1));

				ImGui::Separator();
				//ImGui::SameLine();
				static float position[2] = { 0.0f, 0.0f };

				if (ImGui::SliderFloat2("Position", position, -1000.0f, 1000.0f, "%.f", 1.0f))
				{
					Vector2 pos = Vector2(position[0], position[1]);
					item[amount]->SetPosition(pos);
				}

				if (ImGui::Button("Supply F", ImVec2(47.5f, 25.0f)))
				{
					if (amount < 15)
					{
						if (!item[amount]->IsActive())
						{
							item[amount]->SetPosition(position[0], position[1]);
							item[amount]->Reset();
							cout << amount << "번째 아이템 생성" << endl;
						}
					}
				}

				ImGui::SameLine();
				ImGui::Text("Fixed supply");

				ImGui::Separator();

				static float rangeX[2] = { 0.0f, 0.0f };
				static float rangeY[2] = { 0.0f, 0.0f };

				if (ImGui::SliderFloat2("Range X", rangeX, -1000.0f, 1000.0f, "%.f", 1.0f))
				{

				}

				if (ImGui::SliderFloat2("Range Y", rangeY, -1000.0f, 1000.0f, "%.f", 1.0f))
				{

				}

				if (rangeX[0] >= rangeX[1])
					rangeX[1] = rangeX[0] + 1;
				if (rangeY[0] >= rangeY[1])
					rangeY[1] = rangeY[0] + 1;

				uniform_int_distribution<> distribution1((int)rangeX[0], (int)rangeX[1]);
				uniform_int_distribution<> distribution2((int)rangeY[0], (int)rangeY[1]);
				auto generator1 = bind(distribution1, engine);
				auto generator2 = bind(distribution2, engine);

				//ImGui::SameLine();
				if (ImGui::Button("Supply R", ImVec2(47.5f, 25.0f)))
				{
					if (amount < 15)
					{
						if (!item[amount]->IsActive())
						{
							item[amount]->SetPosition((float)generator1(), (float)generator2());
							item[amount]->Reset();
							cout << amount << "번째 아이템 생성" << endl;
						}
					}
				}

				ImGui::SameLine();
				ImGui::Text("Random Supply");

				ImGui::Separator();

				if (ImGui::Button("Supply P", ImVec2(47.5f, 25.0f)))
				{
					Vector2 pPos = player->GetPosition();
					item[amount]->SetPosition(pPos);
					item[amount]->Reset();
					cout << amount << "번째 아이템 생성" << endl;
				}
				ImGui::SameLine();
				ImGui::Text("Player Pos Supply");

				if (amount == MAX_ITEM - 1)
				{
					ImGui::Text("Max Item Amount!");
				}

				amount = 0;
				ImGui::EndTabItem();
			}


			if (ImGui::BeginTabItem("Brazier"))
			{
				static const char* includes[] =
				{
					"NONE", "EXP", "Coin", "CoinBag", "BigCoinBag", "Chicken",
					"Rosary", "Nduja", "Clock", "Vaccum", "Clover", "Chest"
				};

				static int combo;

				ImGui::Combo("Include", &combo, includes, ARRAYSIZE(includes));

				static UINT amount = 0;

				for (int i = 0; i < MAX_ITEM - 1; i++)
				{
					if (brazier[amount]->IsActive())
						amount++;
					else
						break;
				}

				brazier[amount]->SetInclude((Brazier::Include)(combo - 1));

				ImGui::Separator();
				//ImGui::SameLine();
				static float position[2] = { 0.0f, 0.0f };

				if (ImGui::SliderFloat2("Position", position, -1000.0f, 1000.0f, "%.f", 1.0f))
				{
					brazier[amount]->SetPosition(position[0], position[1]);
				}

				if (ImGui::Button("Supply F", ImVec2(47.5f, 25.0f)))
				{
					if (amount < 15)
					{
						if (!brazier[amount]->IsActive())
						{
							brazier[amount]->SetActive(true);
							brazier[amount]->SetHP(10.0f);
							brazier[amount]->SetPosition(position[0], position[1]);
							cout << amount << "번째 아이템 생성" << endl;
						}
					}
				}

				ImGui::SameLine();
				ImGui::Text("Fixed supply");

				ImGui::Separator();

				static float rangeX[2] = { 0.0f, 0.0f };
				static float rangeY[2] = { 0.0f, 0.0f };

				if (ImGui::SliderFloat2("Range X", rangeX, -1000.0f, 1000.0f, "%.f", 1.0f))
				{

				}

				if (ImGui::SliderFloat2("Range Y", rangeY, -1000.0f, 1000.0f, "%.f", 1.0f))
				{

				}

				if (rangeX[0] >= rangeX[1])
					rangeX[1] = rangeX[0] + 1;
				if (rangeY[0] >= rangeY[1])
					rangeY[1] = rangeY[0] + 1;

				uniform_int_distribution<> distribution1((int)rangeX[0], (int)rangeX[1]);
				uniform_int_distribution<> distribution2((int)rangeY[0], (int)rangeY[1]);
				auto generator1 = bind(distribution1, engine);
				auto generator2 = bind(distribution2, engine);

				//ImGui::SameLine();
				if (ImGui::Button("Supply R", ImVec2(47.5f, 25.0f)))
				{
					if (amount < 15)
					{
						if (!brazier[amount]->IsActive())
						{
							brazier[amount]->SetActive(true);
							brazier[amount]->SetHP(10.0f);
							brazier[amount]->SetPosition((float)generator1(), (float)generator2());
							cout << amount << "번째 화로 생성" << endl;
						}
					}
				}

				ImGui::SameLine();
				ImGui::Text("Random Supply");

				ImGui::Separator();

				if (ImGui::Button("Supply P", ImVec2(47.5f, 25.0f)))
				{
					Vector2 pPos = player->GetPosition();
					brazier[amount]->SetActive(true);
					brazier[amount]->SetHP(10.0f);
					brazier[amount]->SetPosition(pPos);
					cout << amount << "번째 화로 생성" << endl;
				}

				ImGui::SameLine();
				ImGui::Text("Player Pos Supply");

				if (amount == MAX_ITEM - 1)
				{
					ImGui::Text("Max Item Amount!");
				}

				amount = 0;
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Monster"))
			{
				//static const char* types[] =
				//{
				//	"NONE", "BAT", "ZOMBIE", "SKULL", "GHOST", "GHOUL",
				//	"BEAST", "MANTIS", "MUMMY", "NAPENDES", "VINUS"
				//};
				//
				//static int combo = 0;
				//ImGui::Combo("Type", &combo, types, ARRAYSIZE(types));

				ImGui::Text("Status");

				static UINT amount = 0;

				for (int i = 0; i < MAX_MOB; i++)
				{
					if (mob[amount]->IsActive())
					{
						amount++;
					}
					else
						continue;
				}

				static float damage = 1.0f; // damage per sec
				static float hp = 1.0f;
				static float moveSpeed = 50.0f;

				if (ImGui::SliderFloat("Damage/Sec", &damage, 0.0f, 120.0f))
				{
					mob[amount]->SetDamage(damage);
				}
				if (ImGui::SliderFloat("HP", &hp, 0.0f, 500.0f))
				{
					mob[amount]->SetHp(hp);
				}
				if (ImGui::SliderFloat("MoveSpeed", &moveSpeed, 0.0f, 250.0f))
				{
					mob[amount]->SetMoveSpeed(moveSpeed);
				}

				static bool move = true;
				if (ImGui::Checkbox("Move?", &move))
				{
				}

				for (int i = 0; i < MAX_MOB; i++)
				{
					mob[i]->SetMoveSpeed(moveSpeed);
					mob[i]->SetMoving(move);
				}

				ImGui::SameLine();

				static bool col = true;
				if (ImGui::Checkbox("Collision?", &col))
				{
				}
				for (int i = 0; i < MAX_MOB; i++)
					mob[i]->SetIntersect(col);

				//ImGui::SameLine();

				ImGui::Text("Supply");
				ImGui::SameLine();
				static bool jumbo = false;
				if (ImGui::Checkbox("Jumbo?", &jumbo))
				{
					if (jumbo)
						mob[amount]->SetScale(5.0f, 5.0f);
					else
						mob[amount]->SetScale(2.5f, 2.5f);
				}

				ImGui::Separator();

				static float position[2] = { 0.0f, 0.0f };

				if (ImGui::SliderFloat2("Position", position, -1000.0f, 1000.0f, "%.f", 1.0f))
				{
					mob[amount]->SetPosition(position[0], position[1]);
				}

				if (ImGui::Button("Supply F2", ImVec2(47.5f, 25.0f)))
				{
					if (amount < MAX_MOB)
					{
						if (!mob[amount]->IsActive())
						{
							mob[amount]->SetPosition(position[0], position[1]);
							mob[amount]->SetDamage(damage);
							mob[amount]->SetHp(hp);
							mob[amount]->SetMoveSpeed(moveSpeed);
							mob[amount]->Reset();
							cout << amount << "번째 몬스터 생성" << endl;
						}
					}
				}

				ImGui::SameLine();
				ImGui::Text("Fixed supply");

				ImGui::Separator();

				static float rangeX[2] = { 0.0f, 0.0f };
				static float rangeY[2] = { 0.0f, 0.0f };

				if (ImGui::SliderFloat2("Range X", rangeX, -1000.0f, 1000.0f, "%.f", 1.0f))
				{

				}

				if (ImGui::SliderFloat2("Range Y", rangeY, -1000.0f, 1000.0f, "%.f", 1.0f))
				{

				}

				if (rangeX[0] >= rangeX[1])
					rangeX[1] = rangeX[0] + 1;
				if (rangeY[0] >= rangeY[1])
					rangeY[1] = rangeY[0] + 1;

				uniform_int_distribution<> distribution1((int)rangeX[0], (int)rangeX[1]);
				uniform_int_distribution<> distribution2((int)rangeY[0], (int)rangeY[1]);
				auto generator1 = bind(distribution1, engine);
				auto generator2 = bind(distribution2, engine);

				//ImGui::SameLine();
				if (ImGui::Button("Supply R2", ImVec2(47.5f, 25.0f)))
				{
					if (amount < MAX_MOB - 1)
					{
						if (!item[amount]->IsActive())
						{
							mob[amount]->SetPosition((float)generator1(), (float)generator2());
							mob[amount]->SetDamage(damage);
							mob[amount]->SetHp(hp);
							mob[amount]->SetMoveSpeed(moveSpeed);
							mob[amount]->Reset();
							cout << amount << "번째 몬스터 생성" << endl;
						}
					}
				}

				ImGui::SameLine();
				ImGui::Text("Random Supply");

				ImGui::Separator();

				if (ImGui::Button("Supply P2", ImVec2(47.5f, 25.0f)))
				{
					Vector2 pPos = player->GetPosition();
					mob[amount]->SetPosition(pPos);
					mob[amount]->SetDamage(damage);
					mob[amount]->SetHp(hp);
					mob[amount]->SetMoveSpeed(moveSpeed);
					mob[amount]->Reset();
					cout << amount << "번째 몬스터 생성" << endl;
				}
				ImGui::SameLine();
				ImGui::Text("Player Pos Supply");


				if (amount == MAX_MOB - 1)
				{
					ImGui::Text("Max Mob Amount!");
				}

				amount = 0;
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Timer"))
			{
				int min = UI->GetMin();
				int sec = UI->GetSec();

				ImGui::Text("Modify Setting");

				ImGui::Text("Minute");
				ImGui::SameLine();

				static int val1 = 0;
				static int val2 = 0;
				if (ImGui::InputInt("##min", &val1))
				{

				}
				ImGui::SameLine();
				if (ImGui::ArrowButton("MinUp", ImGuiDir_Up))
				{
					min += val1;
					UI->SetMin(min);
				}
				ImGui::SameLine();
				if (ImGui::ArrowButton("MinDown", ImGuiDir_Down))
				{
					min -= val1;
					UI->SetMin(min);
				}

				ImGui::Text("Second");
				ImGui::SameLine();
				if (ImGui::InputInt("##sec", &val2))
				{

				}
				ImGui::SameLine();
				if (ImGui::ArrowButton("SecUp", ImGuiDir_Up))
				{
					sec += val2;
					UI->SetSec(sec);
				}
				ImGui::SameLine();
				if (ImGui::ArrowButton("SecDown", ImGuiDir_Down))
				{
					sec -= val2;
					UI->SetSec(sec);
				}

				ImGui::Separator();
				ImGui::Text("Ratio Setting");

				static float ratio = UI->GetRatio();

				if (ImGui::InputFloat("##Ratio", &ratio))
				{
					UI->SetRatio(ratio);
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}