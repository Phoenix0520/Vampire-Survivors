#include "framework.h"
#include "EquipList.h"
#include "SkillEffect.h"
#include "Player.h"

EquipList::EquipList()
{
	wstring imageFile1 = L"./Images/SkillEffect.png";
	wstring imageFile2 = L"./Images/CollectionTexture/All.png";
	wstring imageFile3 = L"./Images/SkillList0.png";
	wstring imageFile4 = L"./Images/SkillList1.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";
	
	// 플레이어
	{
		player = (Player*)OBJMANAGER->FindObject("Player");
	}

	// 스킬 & 아이템 할당
	{
		AssignSkill("Whip", 10.0f, 1.35f);
		AssignSkill("Magic", 10.0f, 1.25f, 1);
		AssignSkill("Sword", 6.0f, 1.0f, 1);
		AssignSkill("Axe", 20.0f, 4.0f, 2);
		AssignSkill("Cross", 5.0f, 2.0f);
		AssignSkill("Bible", 10.0f, 6.0f, 3.0f);
		AssignSkill("Fire", 20.0f, 3.0f, 1);
		AssignSkill("Garlic", 5.0f, 0.3f);

		AssignItem("Spanish", 10.0f, 5);
		AssignItem("Armor", 1.0f, 5);
		AssignItem("BlackHt", 20.0f, 5);
		AssignItem("RedHt", 0.2f, 5);
		AssignItem("Book", 8.0f, 5);
		AssignItem("Candle", 10.0f, 5);
		AssignItem("Glove", 10.0f, 5);
		AssignItem("Spell", 10.0f, 5);
		AssignItem("Ring", 1.0f, 2);
		AssignItem("Wing", 10.0f, 5);
		AssignItem("Clover", 10.0f, 5);
		AssignItem("Crown", 8.0f, 5);
	}

	// 리스트 출력
	{
		texture[0] = new Texture(imageFile3, shaderFile);
		texture[0]->SetPosition(-547.5f, -2.5f);
		texture[0]->UpdateColorBuffer(Color(), 0, 0, 0, 0);

		texture[1] = new Texture(imageFile4, shaderFile);
		texture[1]->SetPosition(-632.5f, 87.5f);
		texture[1]->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, 0.75f), 13, 0, 0, 0);
	}

	// 아이템 아이콘 렌더링
	{
		wstring imageFile5 = L"./Images/CollectionTexture/All.png";
		wstring imageFile6 = L"./Images/InGameSceneTexture/EquipedTile.png";
		wstring imageFile7 = L"./Images/InGameSceneTexture/LevelVal.png";


		item = new Texture(imageFile5, shaderFile);
		item->SetScale(2.25f, 2.25f);

		tile = new Texture(imageFile6, shaderFile);
		tile->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, 0.5f), 7, 0, 0, 0);
		tile->SetScale(2.275f, 2.275f);
		tile->SetOffsetSize(14.0f, 14.0f);

		levelVal = new Texture(imageFile7, shaderFile);
		levelVal->SetOffsetSize(8.0f, 8.0f);
		levelVal->SetScale(1.75f, 1.75f);
	}
}

EquipList::~EquipList()
{
	for (UINT i = 0; i < skills.size(); i++)
	{
		SAFE_DELETE(skills[i])
	}

	skills.erase(skills.begin(), skills.end());
	skills.clear();

	for (UINT i = 0; i < items.size(); i++)
	{
		SAFE_DELETE(items[i])
	}

	items.erase(items.begin(), items.end());
	items.clear();
}

void EquipList::Update(Matrix V, Matrix P)
{
	switch (type)
	{
	case 0:
		texture[0]->SetPosition(-547.5f + CAMPOS.x, -2.5f + CAMPOS.y);
		texture[0]->Update(V, P);
		break;
	case 1:
		texture[1]->SetPosition(-617.5f + CAMPOS.x, 87.5f + CAMPOS.y);
		texture[1]->Update(V, P);
		tile->Update(V, P);
		levelVal->Update(V, P);
		item->Update(V, P);
		break;
	}
}

void EquipList::Render()
{
	float val = 0.0f;
	float y = 247.5f;
	switch (type)
	{
	case 0:
		texture[0]->Render();
		val = -540.0f;
		break;
	case 1:
	{
		texture[1]->Render();
		val = -610.0f;

		// 상단 획득한 아이템 및 레벨
		{
			Vector2 offset;
			Vector2 offSize;
			Vector2 pos = Vector2(-775.0f, 410.0f);

			int* eSkillList = player->GetEquipedSkillList();
			int* eItemList = player->GetEquipedItemList();

			for (int i = 0; i < 6; i++)
			{
				pos.x += 45.0f;

				if (eSkillList[i] == -1)
					continue;

				ReadTextFile(eSkillList[i], offset, offSize);

				item->SetOffset(offset);
				item->SetOffsetSize(offSize);
				
				tile->SetOffset(0.0f, 0.0f);
				tile->SetPosition(pos + CAMPOS);
				tile->Update(VM, PM);
				tile->Render();

				item->SetPosition(pos + CAMPOS);
				item->Update(VM, PM);
				item->Render();

				Vector2 pos2 = Vector2(pos.x - 12.5f, 380.0f);

				for (int j = 0; j < 8; j++)
				{
					if (GetSkillLevel(eSkillList[i]) > j)
						levelVal->SetOffset(8.0f, 0.0f);
					else
						levelVal->SetOffset(0.0f, 0.0f);

					levelVal->SetPosition(pos2 + CAMPOS);
					levelVal->Update(VM, PM);
					levelVal->Render();

					pos2.x += 12.5f;
					
					if (j == 2 || j == 5)
					{
						pos2.x = pos.x - 12.5f;
						pos2.y -= 12.5f;
					}
				}
			}

			pos = Vector2(-775.0f, 320.0f);

			for (int i = 0; i < 6; i++)
			{
				pos.x += 45.0f;

				if (eItemList[i] == -1)
					continue;

				ReadTextFile(eItemList[i], offset, offSize);

				item->SetOffset(offset);
				item->SetOffsetSize(offSize);

				tile->SetOffset(14.0f, 14.0f);
				tile->SetPosition(pos + CAMPOS);
				tile->Update(VM, PM);
				tile->Render();

				item->SetPosition(pos + CAMPOS);
				item->Update(VM, PM);
				item->Render();

				Vector2 pos2 = Vector2(pos.x - 12.5f, 290.0f);

				for (int j = 0; j < GetItemMaxLevel(eItemList[i] - 8); j++)
				{
					if (GetItemLevel(eItemList[i] - 8) > j)
						levelVal->SetOffset(8.0f, 8.0f);
					else
						levelVal->SetOffset(0.0f, 0.0f);

					levelVal->SetPosition(pos2 + CAMPOS);
					levelVal->Update(VM, PM);
					levelVal->Render();

					pos2.x += 12.5f;

					if (j == 2 || j == 5)
					{
						pos2.x = pos.x - 12.5f;
						pos2.y -= 12.5f;
					}
				}
			}

		}
		break;
	}
	}

	// 하단 능력치 렌더링
	{
		for (int i = 0; i < 19; i++)
		{
			wstring str;
			wstring wVal = L"0";

			float x1 = 0.0f;
			float x2 = 0.0f;

			switch (i)
			{
			case 0:
				str = L"최대 체력";
				x1 = -640.0f + 515.0f;
				x2 = -75.0f;
				wVal = f_to_wstring(player->GetMaxHp());
				break;
			case 1:
				str = L"회복";
				x1 = -632.5f + 515.0f;
				wVal = to_wstring(player->GetRecovery());
				wVal.assign(wVal.begin(), wVal.end() - 5);
				break;
			case 2:
				str = L"방어력";
				x1 = -631.25f + 515.0f;
				x2 = -100.0f;
				wVal = L"+" + f_to_wstring(player->GetDef());
				break;
			case 3:
				str = L"이동속도";
				x1 = -632.5f + 515.0f;
				wVal = L"+" + f_to_wstring(player->GetMoveSpeed() * 100.0f) + L"%";
				break;
			case 4:
				str = L"피해량";
				x1 = -631.25f + 515.0f;
				wVal = L"+" + f_to_wstring(player->GetPower() * 100.0f) + L"%";
				break;
			case 5:
				str = L"공격범위";
				x1 = -632.5f + 515.0f;
				wVal = L"+" + f_to_wstring(player->GetArea() * 100.0f) + L"%";
				break;
			case 6:
				str = L"투사체 속도";
				x1 = -640.0f + 515.0f;
				wVal = L"+" + f_to_wstring(player->GetSkillSpeed() * 100.0f) + L"%";
				break;
			case 7:
				str = L"지속시간";
				x1 = -632.5f + 515.0f;
				wVal = L"+" + f_to_wstring(player->GetDuration() * 100.0f) + L"%";
				break;
			case 8:
				str = L"투사체 수";
				x1 = -638.75f + 515.0f;
				wVal = L"+" + f_to_wstring(player->GetAmount());
				break;
			case 9:
				str = L"쿨타임";
				x1 = -631.25f + 515.0f;
				wVal = L"-" + f_to_wstring(player->GetCoolDown() * 100.0f) + L"%";
				break;
			case 10:
				str = L"행운";
				x1 = -632.5f + 515.0f;
				wVal = L"+" + f_to_wstring(player->GetLuck() * 100.0f) + L"%";
				break;
			case 11:
				str = L"성장";
				x1 = -632.5f + 515.0f;
				wVal = L"+" + f_to_wstring(player->GetGrowth() * 100.0f) + L"%";
				break;
			case 12:
				str = L"탐욕";
				x1 = -632.5f + 515.0f;
				break;
			case 13:
				str = L"저주";
				x1 = -632.5f + 515.0f;
				break;
			case 14:
				str = L"자석";
				x1 = -632.5f + 515.0f;
				x2 = -75.0f;
				wVal = f_to_wstring(player->GetMagnet());
				break;
			case 15:
				str = L"부활";
				x1 = -632.5f + 515.0f;
				break;
			case 16:
				str = L"새로고침";
				x1 = -632.5f + 515.0f;
				break;
			case 17:
				str = L"건너뛰기";
				x1 = -632.5f + 515.0f;
				break;
			case 18:
				str = L"지우기";
				x1 = -631.25f + 515.0f;
				break;
			}
			x2 = -120.0f;

			float x = val + x1;

			Vector2 pos = Vector2(x, y);
			pos += CAMPOS;
			CAMERA->VCToWC(pos);

			DirectWrite::RenderText(str, pos, 255, 255, 255, 20.0f);
			wstring check;
			check.assign(wVal.end() - 1, wVal.end());

			if (wVal == L"0" || wVal == L"0.0" || wVal == L"+0" || wVal == L"+0%" || wVal == L"-0%")
			{
				x = val - x2 - 17.5f;
				pos = Vector2(x, y + 2.5f);
				pos += CAMPOS;
				CAMERA->VCToWC(pos);

				DirectWrite::RenderText(L"-", pos, 255, 255, 255, 20.0f);
			}
			else
			{
				x = val - x2 - wVal.length() * 14.5f;

				if (check == L"%")
					x -= 2.5f;

				pos = Vector2(x, y + 2.5f);
				pos += CAMPOS;
				CAMERA->VCToWC(pos);

				DirectWrite::RenderText(wVal, pos, 255, 255, 0, 20.0f);
			}


			pos -= CAMPOS;

			if (i == 3 || i == 9 || i == 14)
				y -= 45.0f;
			else
				y -= 22.5f;

		}
	}
}

void EquipList::Reset()
{
	for (UINT i = 0; i < skills.size(); i++)
	{
		SAFE_DELETE(skills[i])
	}

	skills.erase(skills.begin(), skills.end());
	skills.clear();
}

void EquipList::AssignSkill(string name, float damage, float coolTime, float duration, int spear)
{
	Skill* skill = new Skill();

	SkillEffect* se = (SkillEffect*)OBJMANAGER->FindObject("SkillEffect" + to_string(skills.size()));
	skill->sEffect = se;
	skill->id = skills.size() - 1;
	skill->name = name;
	skill->damage = damage;
	skill->level = 0;
	skill->amount = 1;
	skill->spear = spear;
	skill->area = 0.0f;
	skill->coolTime = coolTime;
	skill->duration = duration;
	skill->equip = false;

	skills.push_back(skill);
}

void EquipList::AssignItem
(
	string name,
	float value,
	int maxLv
)
{
	Item* item = new Item();

	item->name = name;
	item->value = value;
	item->id = items.size() - 1;
	item->level = 0;
	item->maxLv = maxLv;
	item->equip = false;

	items.push_back(item);
}

void EquipList::ReadTextFile(int row, Vector2& offset, Vector2& offsetSize)
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
			offsetSize.x = v2.x;
			offsetSize.y = v2.y;
		}
	}

	fclose(fp);
}