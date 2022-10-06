#include "framework.h"
#include "ObjectList.h"
#include "GameOverScene.h"
#include "Button.h"

GameOverScene::GameOverScene()
{
	wstring imageFile1 = L"./Images/GameOverScene.png";
	wstring imageFile2 = L"./Images/CollectionTexture/All.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	background = new Texture(imageFile1, shaderFile);

	button = new Button();
	button->SetColor(11);
	button->SetButtonString(L"완료");
	button->SetButtonStringSize(50.0f);

	texture = new Texture(imageFile2, shaderFile);
	texture->SetScale(1.375f, 1.375f);

	sceneName = "GameOverScene";
	SetActive(false);
}

GameOverScene::~GameOverScene()
{
	SAFE_DELETE(button);
	SAFE_DELETE(background);
}

void GameOverScene::Update()
{
	CAMERA->Update();

	Matrix V = CAMERA->GetViewMatrix();
	Matrix P = CAMERA->GetProjMatrix();

	background->SetPosition(CAMPOS);
	background->Update(V, P);

	if (button->IsClick())
	{
		SCENEMANAGER->ChangeScene("LobbyScene");
		button->SetClick(false);
	}

	button->SetHover(true);
	button->SetPosition(CAMPOS.x + 0.0f, CAMPOS.y - 417.5f);
	button->Update(V, P);

	texture->Update(V, P);
}

void GameOverScene::Render()
{
	BEGINDRAW;

	background->Render();
	button->Render();


	wstring str = L"결과";
	float x = -(str.length() * 45.0f * 0.5f);
	Vector2 pos = Vector2(x + 0.0f, 365.0f);
	CAMERA->VCToWC(pos);
	DirectWrite::RenderText(str, pos, 255, 255, 255, 45.0f);

	// Left Side
	{
		str = L"광기의 숲 - 보통 - 골드 x 1";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x + -287.5f, 300.0f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 0, 25.0f);

		str = L"생존 시간 : ";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x + -525.0f, 257.5f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"획득한 골드 : ";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x + -513.5f, 222.5f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"도달한 레벨 :";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x + -513.5f, 187.5f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"처치한 적 :";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x + -525.0f, 152.5f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		// 생존 시간
		{
			str = to_wstring(finalMin) + L":";

			if (finalSec >= 10)
				str += to_wstring(finalSec);
			else
				str += L"0" + to_wstring(finalSec);

			x = -(str.length() * 25.0f * 0.525f);
			pos = Vector2(x + -55.0f, 257.5f);
			CAMERA->VCToWC(pos);
			DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);
		}

		// 획득한 골드
		{
			int val = finalGold;
			str = to_wstring(val);
			x = -(str.length() * 25.0f * 0.5f);
			if (val >= 10000)
				x -= 25.5f;
			else if (val >= 1000)
				x -= 18.5f;
			else if (val >= 100)
				x -= 12.5f;
			else if (val >= 10)
				x -= 5.0f;

			pos = Vector2(x + -40.0f, 222.5f);
			CAMERA->VCToWC(pos);
			DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);
		}

		// 도달한 레벨
		{
			int val = finalLevel;
			str = to_wstring(val);
			x = -(str.length() * 25.0f * 0.5f);
			if (val >= 10000)
				x -= 25.5f;
			else if (val >= 1000)
				x -= 18.5f;
			else if (val >= 100)
				x -= 12.5f;
			else if (val >= 10)
				x -= 5.0f;
			pos = Vector2(x + -40.0f, 187.5f);
			CAMERA->VCToWC(pos);
			DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);
		}

		// 총 처치한 적
		{
			int val = finalKillCount;
			str = to_wstring(val);
			x = -(str.length() * 25.0f * 0.5f);
			if (val >= 1000000)
				x -= 39.5f;
			else if (val >= 100000)
				x -= 33.5f;
			else if (val >= 10000)
				x -= 25.5f;
			else if (val >= 1000)
				x -= 18.5f;
			else if (val >= 100)
				x -= 12.5f;
			else if (val >= 10)
				x -= 5.0f;
			pos = Vector2(x + -40.0f, 152.5f);
			CAMERA->VCToWC(pos);
			DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);
		}

		str = L"무기";
		x = -(str.length() * 22.5f * 0.5f);
		pos = Vector2(x + -552.5f, 107.5f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 0, 22.5f);

		str = L"레벨";
		x = -(str.length() * 22.5f * 0.5f);
		pos = Vector2(x + -362.5f, 107.5f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 0, 22.5f);

		str = L"피해량";
		x = -(str.length() * 22.5f * 0.5f);
		pos = Vector2(x + -287.5f, 107.5f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 0, 22.5f);

		str = L"시간";
		x = -(str.length() * 22.5f * 0.5f);
		pos = Vector2(x + -177.5f, 107.5f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 0, 22.5f);

		str = L"초당피해량";
		x = -(str.length() * 17.5f * 0.5f);
		pos = Vector2(x + -80.0f, 105.0f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 0, 17.5f);

		int* skillList = player->GetEquipedSkillList();
		int* itemList = player->GetEquipedItemList();

		DirectWrite::ChangeTextType
		(
			L"맑은고딕체",
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_CONDENSED
		);

		for (int i = 0; i < 6; i++)
		{
			if (skillList[i] == -1)
				break;
			else
			{
				// 무기 그림
				{
					Vector2 offset = Vector2(0.0f, 0.0f);
					Vector2 offSize = Vector2(0.0f, 0.0f);

					ReadTextFile(skillList[i], offset, offSize);

					texture->SetScale(1.375f, 1.375f);
					texture->SetOffset(offset);
					texture->SetOffsetSize(offSize);
					texture->SetPosition(-590.0f, 66.5f - (28.0f * i));
					texture->Update(VM, PM);
					texture->Render();
				}

				// 무기 종류
				{
					float val = 0.0f;

					switch (skillList[i])
					{
					case 0:
						str = L"채찍";
						break;
					case 1:
						str = L"매직 완드";
						val += 25.0f;
						break;
					case 2:
						str = L"단검";
						break;
					case 3:
						str = L"도끼";
						break;
					case 4:
						str = L"십자가";
						val += 10.0f;
						break;
					case 5:
						str = L"기도문";
						val += 10.0f;
						break;
					case 6:
						str = L"파이어 완드";
						val += 37.0f;
						break;
					case 7:
						str = L"마늘";
						break;
					}

					x = -(str.length() * 22.5f * 0.5f);
					pos = Vector2(x + val + -552.5f, 107.5f);
					pos.y -= 27.5f * (i + 1);

					CAMERA->VCToWC(pos);
					DirectWrite::RenderText(str, pos, 255, 255, 255, 22.5f);
				}

				// 무기 레벨
				{
					str = to_wstring(list->GetSkillLevel(skillList[i]));

					x = -(str.length() * 22.5f * 0.5f);
					pos = Vector2(x + -362.5f, 107.5f);
					pos.y -= 27.5f * (i + 1);

					CAMERA->VCToWC(pos);
					DirectWrite::RenderText(str, pos, 255, 255, 255, 22.5f);
				}

				// 무기 딜량
				{
					float* damageList = player->GetTotalDamageList();
					float totalDamage = damageList[skillList[i]];

					if (totalDamage < 1000.0f)
						str = to_wstring(totalDamage, L"%.1f");
					else if (totalDamage < 1000000.0f)
						str = to_wstring(totalDamage / 1000.0f, L"%.1f") + L"k";
					else
						str = to_wstring(totalDamage / 1000000.0f, L"%.1f") + L"m";

					x = -(str.length() * 22.5f * 0.5f);
					pos = Vector2(x + -287.5f, 107.5f);
					pos.y -= 27.5f * (i + 1);

					CAMERA->VCToWC(pos);

					bool strongest = true;
					for (int i = 0; i < 8; i++)
					{
						if (totalDamage < damageList[i])
						{
							strongest = false;
							break;
						}
					}

					if (strongest)
						DirectWrite::RenderText(str, pos, 255, 255, 0, 22.5f);
					else
						DirectWrite::RenderText(str, pos, 255, 255, 255, 22.5f);
				}

				// 무기 사용 시간
				{
					int min = finalMin - player->GetPrimaryEquipMin()[skillList[i]];
					int sec = finalSec - player->GetPrimaryEquipSec()[skillList[i]];

					if (sec >= 10)
						str = to_wstring(min) + L":" + to_wstring(sec);
					else
						str = to_wstring(min) + L":0" + to_wstring(sec);

					x = -(str.length() * 22.5f * 0.5f);
					pos = Vector2(x + -175.0f, 107.5f);
					pos.y -= 27.5f * (i + 1);

					CAMERA->VCToWC(pos);
					DirectWrite::RenderText(str, pos, 255, 255, 255, 22.5f);
				}

				// 무기 DPS (Damage Per Seconds)
				{
					int min = finalMin - player->GetPrimaryEquipMin()[skillList[i]];
					int sec = finalSec - player->GetPrimaryEquipSec()[skillList[i]];

					int time = min * 60 + sec;

					float* damageList = player->GetTotalDamageList();
					float totalDamage = damageList[skillList[i]];

					float DPS = totalDamage / (float)time;

					str = to_wstring(DPS, L"%.1f");

					x = -(str.length() * 22.5f * 0.5f);
					pos = Vector2(x + -80.0f, 107.5f);
					pos.y -= 27.5f * (i + 1);

					CAMERA->VCToWC(pos);

					bool strongest = true;
					for (int i = 0; i < 8; i++)
					{
						if (totalDamage < damageList[i])
						{
							strongest = false;
							break;
						}
					}

					if (strongest)
						DirectWrite::RenderText(str, pos, 255, 255, 0, 22.5f);
					else
						DirectWrite::RenderText(str, pos, 255, 255, 255, 22.5f);
				}
			}
		}

		DirectWrite::ChangeTextType
		(
			L"맑은고딕체",
			DWRITE_FONT_WEIGHT_SEMI_BOLD,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_CONDENSED
		);
	}

	// Right Side
	{
		int* skillList = player->GetEquipedSkillList();
		int* itemList = player->GetEquipedItemList();

		str = L"안토니오 벨파에제";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x + 312.5f, 272.5f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 0, 25.0f);

		// 무기 그림
		{
			Vector2 offset = Vector2(0.0f, 0.0f);
			Vector2 offSize = Vector2(0.0f, 0.0f);

			int skillSize = 0;
			int itemSize = 0;
			int totalSize = 0;

			for (int i = 0; i < 6; i++)
			{
				if (skillList[i] != -1)
					skillSize++;
			}
			for (int i = 0; i < 6; i++)
			{
				if (itemList[i] != -1)
					itemSize++;
			}

			totalSize = skillSize + itemSize;

			float value = 60.0f - (float)totalSize * 2;
			float interval = totalSize * value / 2;

			for (int i = 0; i < skillSize; i++)
			{
				ReadTextFile(skillList[i], offset, offSize);

				texture->SetScale(1.585f, 1.585f);
				texture->SetOffset(offset);
				texture->SetOffsetSize(offSize);
				texture->SetPosition(317.5f - interval + (value * i), 190.0f);
				texture->Update(VM, PM);
				texture->Render();
			}

			for (int i = 0; i < itemSize; i++)
			{
				ReadTextFile(itemList[i], offset, offSize);

				texture->SetScale(1.585f, 1.585f);
				texture->SetOffset(offset);
				texture->SetOffsetSize(offSize);
				texture->SetPosition(317.5f - interval + (value * (i + skillSize)), 190.0f);
				texture->Update(VM, PM);
				texture->Render();
			}


			for (int i = 0; i < skillSize; i++)
			{
				if (skillList[i] != -1)
					str = to_wstring(list->GetEquipLevel(skillList[i]));

				pos = Vector2(310.0f - interval + (value * i), 172.5f);
				CAMERA->VCToWC(pos);
				DirectWrite::RenderText(str, pos, 255, 255, 255, 22.5f);
			}

			for (int i = 0; i < itemSize; i++)
			{
				if (itemList[i] != -1)
					str = to_wstring(list->GetEquipLevel(itemList[i]));

				pos = Vector2(310.0f - interval + (value * (i + skillSize)), 172.5f);
				CAMERA->VCToWC(pos);
				DirectWrite::RenderText(str, pos, 255, 255, 255, 22.5f);
			}
		}

		str = L"v0.5.0e - EA";
		x = -(str.length() * 20.0f * 0.5f);
		pos = Vector2(x + 540.0f, -335.0f);
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 255, 20.0f);
	}

	ENDDRAW;

	ImGui::GetIO().NavActive = false;
	ImGui::GetIO().WantCaptureMouse = true;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("TEST");

	UINT amount = 0;

	for (int i = 0; i < 6; i++)
	{
		if (player->GetEquipedSkillList()[i] != -1)
			amount++;
	}

	if (ImGui::BeginTabBar("##TabBar1"))
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
						if (!list->GetSkillEquip(id))
						{
							if (list->GetSkillLevel(id) <= 0)
								list->UpdateSkillLevel(id, 1);

							list->UpdateSkillEquip(id, true);
							player->SetEquipedSkill(amount, id);
						}
				}
				ImGui::SameLine();
				if (ImGui::Button("MAGIC Wand"))
				{
					int id = S_MAGIC;

					if (amount != 6)
						if (!list->GetSkillEquip(id))
						{
							if (list->GetSkillLevel(id) <= 0)
								list->UpdateSkillLevel(id, 1);

							list->UpdateSkillEquip(id, true);
							player->SetEquipedSkill(amount, id);

						}
				}
				ImGui::SameLine();
				if (ImGui::Button("SWORD"))
				{
					int id = S_SWORD;

					if (amount != 6)
						if (!list->GetSkillEquip(id))
						{
							if (list->GetSkillLevel(id) <= 0)
								list->UpdateSkillLevel(id, 1);

							list->UpdateSkillEquip(id, true);
							player->SetEquipedSkill(amount, id);

						}
				}
				ImGui::SameLine();
				if (ImGui::Button("AXE"))
				{
					int id = S_AXE;

					if (amount != 6)
						if (!list->GetSkillEquip(id))
						{
							if (list->GetSkillLevel(id) <= 0)
								list->UpdateSkillLevel(id, 1);

							list->UpdateSkillEquip(id, true);
							player->SetEquipedSkill(amount, id);
						}
				}

				if (ImGui::Button("CROSS"))
				{
					int id = S_CROSS;

					if (amount != 6)
						if (!list->GetSkillEquip(id))
						{
							if (list->GetSkillLevel(id) <= 0)
								list->UpdateSkillLevel(id, 1);

							list->UpdateSkillEquip(id, true);
							player->SetEquipedSkill(amount, id);
						}
				}
				ImGui::SameLine();
				if (ImGui::Button("BIBLE"))
				{
					int id = S_BIBLE;

					if (amount != 6)
						if (!list->GetSkillEquip(id))
						{
							if (list->GetSkillLevel(id) <= 0)
								list->UpdateSkillLevel(id, 1);

							list->UpdateSkillEquip(id, true);
							player->SetEquipedSkill(amount, id);
						}
				}
				ImGui::SameLine();
				if (ImGui::Button("FIRE Wand"))
				{
					int id = S_FIRE;

					if (amount != 6)
						if (!list->GetSkillEquip(id))
						{
							if (list->GetSkillLevel(id) <= 0)
								list->UpdateSkillLevel(id, 1);

							list->UpdateSkillEquip(id, true);
							player->SetEquipedSkill(amount, id);
						}
				}
				ImGui::SameLine();
				if (ImGui::Button("GARLIC"))
				{
					int id = S_GARLIC;

					if (amount != 6)
						if (!list->GetSkillEquip(id))
						{
							if (list->GetSkillLevel(id) <= 0)
								list->UpdateSkillLevel(id, 1);

							list->UpdateSkillEquip(id, true);
							player->SetEquipedSkill(amount, id);
						}
				}

				ImGui::EndListBox();

				ImGui::SameLine();
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
							list->UpdateSkillEquip(i, false);
							list->UpdateSkillLevel(i, 0);
						}
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("All MaxLv", ImVec2(75.0f, 25.0f)))
				{
					cout << "Equipment All Max Level!" << endl;
					for (int i = 0; i < 8; i++)
						list->UpdateSkillLevel(i, 8);
					for (int i = 0; i < 12; i++)
						list->UpdateItemLevel(i, 5);
				}
			}
		}
		ImGui::EndTabItem();

		ImGui::EndTabBar();
	}




	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void GameOverScene::ChangeScene()
{
	CAMERA->SetObject(nullptr);
	CAMERA->SetPosition(0.0f, 0.0f);

	player = PLAYER;
	list = (EquipList*)OBJMANAGER->FindObject("EquipList");

	SetProperty(player->GetFinalMin(), player->GetFinalSec(), player->GetLevel(), player->GetCoin(), player->GetKill());
}

void GameOverScene::SetProperty(int min, int sec, int level, int gold, int kill)
{
	finalMin = min;
	finalSec = sec;
	finalLevel = level;
	finalGold = gold;
	finalKillCount = kill;
}

void GameOverScene::ReadTextFile(int row, Vector2 & offset, Vector2 & offSize)
{
	FILE* fp = fopen("./Offset2.txt", "rt");

	if (!fp)
	{
		return;
	}
	char buf[1000];
	int check;

	while (true)
	{
		if (!fgets(buf, 1000, fp))
			break;

		Vector2 v1;
		Vector2 v2;

		sscanf(buf, "%d %f %f %f %f", &check, &v1.x, &v1.y, &v2.x, &v2.y);

		if (check == row)
		{
			offset.x = v1.x;
			offset.y = v1.y;
			offSize.x = v2.x;
			offSize.y = v2.y;
		}
	}

	fclose(fp);
}
