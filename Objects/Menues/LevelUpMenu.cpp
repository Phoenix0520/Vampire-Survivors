#include "framework.h"
#include "LevelUpMenu.h"
#include "ObjectList.h"
#include "Convert.h"
#include "CSVReader.h"

LvUpMenu::LvUpMenu()
{
	wstring imageFile1 = L"./Images/InGameSceneTexture/LevelUpMenu/LevelUpMenu.png";
	wstring imageFile2 = L"./Images/InGameSceneTexture/LevelUpMenu/LevelUpMenuButton1.png";
	wstring imageFile3 = L"./Images/CollectionTexture/All.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	// Window
	{
		window = new Texture(imageFile1, shaderFile);
	}

	// Button
	{
		button = new Texture(imageFile2, shaderFile);
	}

	// Item
	{
		item = new Texture(imageFile3, shaderFile);
		item->SetScale(2.875f, 2.875f);

		SetProperty();
	}

	// 커서
	{
		cursor = (Cursor*)OBJMANAGER->FindObject("Cursor");
	}

	// Skill List
	{
		list = (EquipList*)OBJMANAGER->FindObject("EquipList");
	}

	// InGameUI
	{
		UI = (InGameUI*)OBJMANAGER->FindObject("InGameUI");
	}

	// LvUpEffect
	{
		//effect = (LvUpEffect*)OBJMANAGER->FindObject("LvUpEffect");
	}
}

LvUpMenu::~LvUpMenu()
{
	SAFE_DELETE(list);
	SAFE_DELETE(item);
	SAFE_DELETE(cursor);
	SAFE_DELETE(button);
	SAFE_DELETE(window);
}

void LvUpMenu::Update(Matrix V, Matrix P)
{
	Player* player = PLAYER;

	//effect->Update(V, P);
	window->SetPosition(CAMPOS);
	window->Update(V, P);

	if (DOWN(VK_SPACE) || DOWN(VK_RETURN))
	{
		select = true;

		// Skill Choosed
		
		Upgrade(way[hover]);

		// 컬렉션 등록

		ListCtrl* lc = (ListCtrl*)OBJMANAGER->FindObject("ListCtrl");

		lc->UpdateItemCollectEx(way[hover], true);

		return;
	}

	Vector2 pos;
	switch (hover)
	{
	case 0:
		if (DOWN(VK_DOWN) || DOWN('S'))
			if (wayCount >= 2)
				hover = 1;

		pos = CAMPOS;
		pos.y += 170.0f;
		break;
	case 1:
		if (DOWN(VK_UP) || DOWN('W'))
			hover = 0;
		if (DOWN(VK_DOWN) || DOWN('S'))
			if (wayCount >= 3)
				hover = 2;

		pos = CAMPOS;
		pos.y += 27.5f;
		break;
	case 2:
		if (DOWN(VK_UP) || DOWN('W'))
			hover = 1;
		if (DOWN(VK_DOWN) || DOWN('S'))
			if (wayCount >= 4)
				hover = 3;

		pos = CAMPOS;
		pos.y -= 125.0f;
		break;

	case 3:
		if (DOWN(VK_UP) || DOWN('W'))
			hover = 2;

		pos = CAMPOS;
		pos.y -= 277.5f;
		break;
		
	}
	cursor->SetPosition(pos);

	cursor->SetType(0);
	cursor->SetGap(button->GetTextureRealSize().x / 2 + 27.5f);

	cursor->Update(V, P);
	button->Update(V, P);
}

void LvUpMenu::Render()
{
	//effect->Render();
	window->Render();

	float ny = 390.0f;
	float py = 325.0f;
	for (int i = 0; i < wayCount; i++)
	{
		float y = 0.0f;
		float y2 = 0.0f;

		Vector2 offset = Vector2(0.0f, 0.0f);
		Vector2 offSize = Vector2(0.0f, 0.0f);

		switch (i)
		{
		case 0:
			y = 145.0f;
			y2 = 205.0f;
			break;
		case 1:
			y = -7.5f;
			y2 = 52.5f;
			break;
		case 2:
			y = -160.0f;
			y2 = -99.0f;
			break;
		case 3:
			y = -312.5f;
			y2 = -252.5f;
			break;
		}

		y += CAMPOS.y + 22.5f;
		y2 += CAMPOS.y;

		button->SetPosition(CAMPOS.x, y);
		button->Update(VM, PM);
		button->Render();

		ReadTextFile(way[i], offset, offSize);
		item->SetOffset(offset);
		item->SetOffsetSize(offSize);
		item->SetPosition(CAMPOS.x - 235.0f, y2);
		item->Update(VM, PM);
		item->Render();

		wstring wstr = explains[way[i] * 10];
		float x = -(wstr.length() * 22.5f * 0.5f);
		ny -= button->GetTextureRealSize().y;
		Vector2 pos = Vector2(x, ny);
		pos += CAMPOS;
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(wstr, pos, 255, 255, 255, 22.5f);
		
		bool overlap = false;
		for (int j = 0; j < 6; j++)
		{
			if (equipSkillList[j] == way[i])
			{
				overlap = true;
				break;
			}
			if (equipItemList[j] == way[i])
			{
				overlap = true;
				break;
			}
		}

		if (!overlap)
		{
			wstr = L"신규 !";
			x = (wstr.length() * 22.5f * 1.65f);
			pos = Vector2(x, ny);
			pos += CAMPOS;
			CAMERA->VCToWC(pos);
			DirectWrite::RenderText(wstr, pos, 255, 255, 0, 22.5f);
		}
		else
		{
			if (way[i] < 8)
				wstr = L"레벨 : " + to_wstring(list->GetSkillLevel(way[i]) + 1);
			else
				wstr = L"레벨 : " + to_wstring(list->GetItemLevel(way[i] - 8) + 1);
			
			x = (wstr.length() * 22.5f * 1.0f);
			pos = Vector2(x, ny);
			pos += CAMPOS;
			CAMERA->VCToWC(pos);
			DirectWrite::RenderText(wstr, pos, 255, 255, 255, 22.5f);
		}

		int index = way[i] * 10 + list->GetEquipLevel(way[i]) + 1;
		
		wstr = explains[index];

		x = -(wstr.length() * 22.5f * 0.5f);
		py -= button->GetTextureRealSize().y;
		pos = Vector2(x, py);
		pos += CAMPOS;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(wstr, pos, 255, 255, 255, 22.5f);
	}

	wstring str = L"레벨 업!";
	float x = -(str.length() * 45.0f * 0.5f);
	float y = 90.0f;
	Vector2 pos = Vector2(x, y + 260.0f);
	pos += CAMPOS;
	CAMERA->VCToWC(pos);	

	DirectWrite::RenderText(str, pos, 255, 255, 255, 45.0f);

	if (wayCount == 3)
	{
		str = L"행운 수치가 높으면";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x, y - 345.0f);
		pos += CAMPOS;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"4번째 선택지를 볼 수 있습니다.";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x, y - 395.0f);
		pos += CAMPOS;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);
	}

	cursor->Render();
}

void LvUpMenu::Reset()
{
	//effect->Reset();
	PLAYSOUND("LevelUp", sfxSize);
	select = false;
	hover = 0;
	Random();
}

void LvUpMenu::Random()
{
	time += DELTA;

	Player* pl = PLAYER;

	float luck = pl->GetLuck() * 100.0f;

	mt19937	engine((unsigned int)std::time(NULL));
	uniform_int_distribution<> distribution(0, 100);
	auto generator = bind(distribution, engine);
	
	// 확률:		   luck
	// 100 * ________________
	//			100 + luck

	// 행운 수치에 따른 메뉴 갯수
	{
		int val = generator();

		cout << "행운 계수 : " << (100 * (luck / (100 + luck))) << "%" << endl << endl;

		if (val <= 100 * (luck / (100 + luck)))
			wayCount = 4;
		else
			wayCount = 3;
	}

	// 장착된 스킬 및 아이템 특정
	equipSkillList = pl->GetEquipedSkillList();
	equipItemList = pl->GetEquipedItemList();

	int skillAmount = 0;
	int itemAmount = 0;

	for (int i = 0; i < 6; i++)
	{
		if (equipSkillList[i] != -1)
			skillAmount++;
		if (equipItemList[i] != -1)
			itemAmount++;
	}

	// 랜덤 등장 구성

	cout << "===============[경우]===============" << endl;

	cout << "스킬 갯수 : "<< skillAmount << endl;
	cout << "아이템 갯수 : "<< itemAmount << endl;

	for (int i = 0; i < 20; i++)
	{
		if (i < 8) // 스킬일 시
		{
			if (list->GetSkillLevel(i) >= 8) // 스킬레벨이 최대면 등장하지 않음
			{
				cout << "스킬 " << i << "은(는) 이미 최대레벨입니다. 등장하지 않습니다." << endl;
				continue;
			}

			if (skillAmount == 6) // 스킬이 6개 가 되면
			{
				bool exist = false;

				for (int j = 0; j < 6; j++)	// 보유한 스킬 들중
				{
					if (equipSkillList[j] == i) // 일치하는것만 나타나게 한다.
					{
						cout << j << " 번째로 선택된 스킬 " << i << " 등장" << endl;
						exist = true;
					}
				}

				if (!exist)
					continue;
			}
		}
		else // 아이템일시
		{
			if (list->GetItemLevel(i - 8) >= list->GetItemMaxLevel(i - 8)) // 아이템 레벨이 최대면 등장하지 않음
			{
				cout << "아이템 " << i << "은(는) 이미 최대레벨입니다. 등장하지 않습니다." << endl;
				continue;
			}

			if (itemAmount == 6) // 아이템이 6개 가 되면
			{
				bool exist = false;

				for (int j = 0; j < 6; j++) // 보유한 아이템중
				{
					if (equipItemList[j] == i) // 일치하는것만 나타나게 한다.
					{
						cout << j << " 번째로 선택된 아이템 " << i << " 등장" << endl;
						exist = true;
					}
				}

				if (!exist)
					continue;
			}
		}
		
		random.push_back(i); // 조건에 맞는것만 리스트에 추가한다.
	}

	if (random.size() < 1 || random.size() > 20)
	{
		wayCount = 2;
		way[0] = 20;
		way[1] = 21;

		return;
	}

	cout << "=============[선택 가능]=============" << endl;

	for (UINT i = 0; i < random.size(); i++)
	{
		if (i != random.size() - 1)
			cout << random[i] << ", ";
		else
			cout << random[i] << endl;
	}

	if (random.size() == 0) // 선택지 갯수가 없을시 모든 아이템이 강화되었다는 신호를보낸다.
	{
		maxEnchant = true;
		return;
	}

	uniform_int_distribution<> distribution1(0, random.size() - 1); // 리스트의 갯수만큼의 난수생성기 설정
	auto generator1 = bind(distribution1, engine);

	if ((int)random.size() < wayCount) // 선택지보다 나올수 있는 수가 적을시 선택지 갯수를 맞춘다.
	{
		wayCount = (int)random.size();
	}

	int overlap[4] = { 0, 0, 0, 0 };

	cout << "===============[결과]===============" << endl;
	for (int i = 0; i < wayCount; i++) // 선택지의 갯수만큼
	{
		int val;
		
		switch (i) // 중복체크
		{
		case 0:
		{
			val = random[generator1()];
			overlap[i] = val;
			break;
		}
		case 1:
		{
			do
			{
				val = random[generator1()];
				overlap[i] = val;
			} while (val == overlap[0]);
			break;
		}
		case 2:
		{
			do
			{
				val = random[generator1()];
				overlap[i] = val;
			} while (val == overlap[0] || val == overlap[1]);
			break;
		}
		case 3:
		{
			do
			{
				val = random[generator1()];
				overlap[i] = val;
			} while (val == overlap[0] || val == overlap[1] || val == overlap[2]);
			break;
		}
		}
			
		way[i] = val; // 난수 생성기가 설정된대로 선택지를 정한다.
		
		cout << "선택지 [" << i << "] : " << way[i] << endl;
	}

	random.clear();
}

void LvUpMenu::SetProperty()
{
	CSVReader* reader = new CSVReader();
	reader->OpenFile("./SkillExplain.csv");

	for (UINT i = 0; i < 23; i++)
	{
		for (UINT j = 0; j < 10; j++)
		{
			wstring val = L"";
			reader->GetData(i, j, val);

			explains.push_back(val);
		}
	}
	
	reader->OpenFile("./SkillLevelValue.csv");

	for (UINT i = 0; i < 8; i++)
	{
		for (UINT j = 0; j < 8; j++)
		{
			string str = "";

			reader->GetData(i, j, str);
			addType.push_back(str);
		}
	}
}

void LvUpMenu::Upgrade(int type)
{
	Player* player = PLAYER;

	if (type < 8)
	{
		if (!list->GetSkillEquip(type))
		{
			list->UpdateSkillEquip(type, true);
			list->UpdateSkillLevel(type, 1);

			int min = UI->GetMin();
			int sec = UI->GetSec();

			player->SetPrimaryEquipMin(type, min);
			player->SetPrimaryEquipSec(type, sec);
		}
		else
		{
			int lv = list->GetSkillLevel(type);

			IncreasingItemValue(type, lv);
			list->UpdateSkillLevel(type, lv + 1);
		}

		int count = 0;

		for (int i = 0; i < 6; i++)
		{
			if (equipSkillList[i] != -1)
				count++;
			if (equipSkillList[i] == type)
				return;
		}


		player->SetEquipedSkill(count, type);
	}
	// Item Choosed
	else if (type < 20)
	{
		player->ApplyItem(type - 8);

		if (!list->GetItemEquip(type - 8))
		{
			list->UpdateItemEquip(type - 8, true);
			list->UpdateItemLevel(type - 8, 1);
		}
		else
		{
			int lv = list->GetItemLevel(type - 8);
			list->UpdateItemLevel(type - 8, lv + 1);
		}

		int count = 0;

		for (int i = 0; i < 6; i++)
		{
			if (equipItemList[i] != -1)
				count++;
			if (equipItemList[i] == type)
				return;
		}

		player->SetEquipedItem(count, type);
	}
	else
	{
		if (type == 20) // 동전 가방을 고른경우
			player->SetCoin(player->GetCoin() + 25);
		else // 치킨을 고른경우
			player->EatChicken();

		return;
	}
}

void LvUpMenu::ReadTextFile(int row, Vector2& offset, Vector2& offsetSize)
{
	FILE* fp = fopen("./Offset2.txt", "rt");

	if (!fp)
	{
		cout << "오류!" << endl;
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

void LvUpMenu::IncreasingItemValue(int id, int level)
{
	string input[2] = { addType[level * 8 + id] , addType[level * 8 + id] };
	char* type[2] = { new char(), new char() };
	float value[2] = { 0.0f, 0.0f };
	UINT amount = 0;

	if (strchr(input[amount].c_str(), '|') == nullptr)
	{
		value[amount] = strtof(input[amount].c_str(), &type[amount]);
		cout << "Input : " << input[amount] << " Type : " << type[amount] << " Value : " << value[amount] << endl;

		amount++;
	}
	else
	{
		for (UINT i = 0; i < 2; i++)
		{
			if (amount == 0)
			{
				char* temp = (char*)input[amount].c_str();
				strtok(temp, "|");
				value[amount] = strtof(temp, &type[amount]);
				cout << "First | Input : " << temp[amount] << " Type : " << type[amount] << " Value : " << value[amount] << endl;
			}
			else
			{
				input[amount] = strchr(input[amount].c_str(), '|') + 1;
				value[amount] = strtof(input[amount].c_str(), &type[amount]);
				cout << "Second | Input : " << input[amount] << " Type : " << type[amount] << " Value : " << value[amount] << endl;
			}

			amount++;
		}
	}

	for (UINT i = 0; i < amount; i++)
	{
		switch (type[i][0])
		{
		case 'A':
			list->AddSkillAmount(id, (int)value[amount]);
			cout << value[amount] << " 만큼 투사체 수 증가!" << endl;
			break;
		case 'B':
			list->AddSkillArea(id, value[amount]);
			cout << value[amount] << " 만큼 스킬 범위 증가!" << endl;
			break;
		case 'C':
			list->AddSkillDuration(id, value[amount]);
			cout << value[amount] << " 만큼 스킬 지속시간 증가!" << endl;
			break;
		case 'D':
			list->AddSkillSpear(id, (int)value[amount]);
			cout << value[amount] << " 만큼 스킬 관통 횟수 증가!" << endl;
			break;
		case 'E':
			list->AddSkillSpeed(id, value[amount]);
			cout << value[amount] << " 만큼 스킬 속도 증가!" << endl;
			break;
		case 'F':
			list->AddSkillDamage(id, value[amount]);
			cout << value[amount] << " 만큼 스킬 데미지(공격력) 증가!" << endl;
			break;
		case 'G':
			list->AddSkillCoolDown(id, value[amount]);
			cout << value[amount] << " 만큼 스킬 쿨다운 증가!" << endl;
			break;
		default:
			cout << "오류!" << endl;
			break;
		}
	}
}

