#include "framework.h"
#include "InGameUI.h"
#include "Player.h"
#include "EquipList.h"

InGameUI::InGameUI()
{
	wstring imageFile1 = L"./Images/Background/BackGround.png";
	wstring imageFile2 = L"./Images/BackGround/BG (1).png";
	wstring imageFile3 = L"./Images/BackGround/BG (2).png";
	wstring imageFile4 = L"./Images/BackGround/BG (3).png";

	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	// Interface
	{
		imageFile1 = L"./Images/InGameSceneTexture/KillCount.png";
		imageFile2 = L"./Images/InGameSceneTexture/Gold.png";

		texture[0] = new Texture(imageFile1, shaderFile);
		texture[0]->SetScale(2.15f, 2.15f);
		texture[1] = new Texture(imageFile2, shaderFile);
		texture[1]->SetScale(2.15f, 2.15f);
		imageFile4 = L"./Images/InGameSceneTexture/EquipedSkill.png";

		equipedBox = new Texture(imageFile4, shaderFile);
		equipedBox->SetScale(2.1f, 2.1f);
		equipedBox->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, 0.33f), 7, 0, 0, 0);

		imageFile4 = L"./Images/CollectionTexture/All.png";
		item = new Texture(imageFile4, shaderFile);
		item->SetScale(1.875f, 1.875f);
	}

	// Level
	{
		imageFile3 = L"./Images/InGameSceneTexture/LevelBar.png";

		levelBar = new Texture(imageFile3, shaderFile);

		imageFile4 = L"./Images/InGameSceneTexture/Level.png";

		levelVal = new Texture(imageFile4, shaderFile);
	}
}

InGameUI::~InGameUI()
{
	SAFE_DELETE(item);
	SAFE_DELETE(equipedBox);
	SAFE_DELETE(levelVal);
	SAFE_DELETE(levelBar);
	SAFE_DELETE(texture[0]);
	SAFE_DELETE(texture[1]);
}

void InGameUI::Update(Matrix V, Matrix P)
{
	Player* player = PLAYER;

	// 레벨 바 업데이트
	if (player->GetLvUpRemain() > 0)
	{
		if (plus)
			ctime += DELTA * 2.5f;
		else
			ctime -= DELTA * 2.5f;

		if (ctime > 1.0f)
			plus = false;
		if (ctime < 0.0f)
			plus = true;

		levelVal->UpdateColorBuffer(Color(0.0f, 0.0f, ctime, 0.0f), 11, ctime, 0, 0);
		levelVal->SetScale(1.0f, 1.0f);
	}

	// 시간 업데이트
	if (time >= 60.0f)
	{
		min++;
		time = 0.0f;
	}
	else
	{
		sec = (int)time;
		time += DELTA * ratio;
	}

	if (!active)
		return;

	Vector2 pos = Vector2(505.0f, 427.5f);

	pos += CAMPOS;

	texture[0]->SetPosition(pos);

	pos.x += 235.0f;
	texture[1]->SetPosition(pos);

	texture[0]->Update(V, P);
	texture[1]->Update(V, P);

	pos = Vector2(0.0f, 460.25f);

	levelBar->SetPosition(pos + CAMPOS);
	levelBar->Update(V, P);
	
	if (player->GetLvUpRemain() > 0)
	{
		levelVal->SetScale(1.0f, 1.0f);
	}
	else
	{
		float x = player->GetExp() / 100.0f;
		levelVal->SetScale(x, 1.0f);
		levelVal->UpdateColorBuffer(Color(), 0, 0, 0, 0);
	}

	pos.x = levelVal->GetTextureRealSize().x / 2 - 760.0f;

	levelVal->SetPosition(pos + CAMPOS);
	levelVal->Update(V, P);

	pos = Vector2(-665.0f, 408.0f);

	equipedBox->SetPosition(pos + CAMPOS);
	equipedBox->Update(V, P);

	item->Update(V, P);
}

void InGameUI::Render()
{
	Player* player = (Player*)OBJMANAGER->FindObject("Player");
	EquipList* list = (EquipList*)OBJMANAGER->FindObject("EquipList");
	texture[0]->Render();
	texture[1]->Render();

	levelBar->Render();
	levelVal->Render();

	equipedBox->Render();

	int* skillList = player->GetEquipedSkillList();
	int* itemList = player->GetEquipedItemList();

	Vector2 pos = Vector2(-781.5f, 402.5f);
	Vector2 offset = Vector2(0.0f, 0.0f);
	Vector2 offSize = Vector2(0.0f, 0.0f);

	// Equiped Item Render
	for (int i = 0; i < 6; i++)
	{
		pos.x += 33.5f;

		if (skillList[i] == -1)
			break;

		ReadTextFile(skillList[i], offset, offSize);

		item->SetOffset(offset);
		item->SetOffsetSize(offSize);
		item->SetPosition(pos.x + CAMPOS.x, pos.y + CAMPOS.y + 22.5f);
		item->Update(VM, PM);
		item->Render();
	}

	pos = Vector2(-782.5f, 370.0f);

	for (int i = 0; i < 6; i++)
	{
		pos.x += 33.5f;

		if (itemList[i] == -1)
			break;

		ReadTextFile(itemList[i], offset, offSize);
		
		item->SetOffset(offset);
		item->SetOffsetSize(offSize);
		item->SetPosition(pos.x + CAMPOS.x, pos.y + CAMPOS.y + 22.5f);
		item->Update(VM, PM);
		item->Render();
	}

	{
		// 시간
		
		wstring str = to_wstring(min) + L" : " + to_wstring(sec);

		if (sec < 10)
			str = to_wstring(min) + L" : 0" + to_wstring(sec);
		float x = -(str.length() * 40.0f * 0.5f);

		Vector2 pos = Vector2(x, 440.0f);
		Vector2 pos2 = CAMERA->GetPosition();

		pos += pos2;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 40.0f);

		// 킬수
		int kill = player->GetKill();

		str = to_wstring(kill);
		pos = Vector2(460.0f, 445.0f);

		if (kill >= 10000)
			pos.x -= 55.0f;
		else if (kill >= 1000)
			pos.x -= 40.0f;
		else if (kill >= 100)
			pos.x -= 25.0f;
		else if (kill >= 10)
			pos.x -= 10.0f;

		pos += CAMERA->GetPosition();
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		// 금화 수
		int gold = player->GetCoin();

		str = to_wstring(gold);
		pos = Vector2(685.0f, 445.0f);

		if (gold >= 1000)
			pos.x -= 40.0f;
		else if (gold >= 100)
			pos.x -= 25.0f;
		else if (gold >= 10)
			pos.x -= 10.0f;

		pos += CAMERA->GetPosition();
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		Player* pl = PLAYER;
		int level = pl->GetLevel();
		str = L"LV   " + to_wstring(level);
		pos = Vector2(650.0f, 477.5f);

		if (level >= 100)
			pos.x -= 35.0f;
		else if (level >= 10)
			pos.x -= 17.5f;

		pos += CAMERA->GetPosition();
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);
	}
}

void InGameUI::Reset()
{
	min = 0;
	sec = 0;
	time = 0.0f;
	ctime = 0.0f;
	plus = true;
}

void InGameUI::ReadTextFile(int row, Vector2 & offset, Vector2 & offsetSize)
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