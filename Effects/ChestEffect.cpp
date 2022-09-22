#include "framework.h"
#include "ChestEffect.h"
#include "Player.h"
#include "LevelUpMenu.h"
#include "EquipList.h"

ChestEffect::ChestEffect()
{
	wstring imageFile1 = L"./Images/Chest/OpenEffect1.png";
	wstring imageFile2 = L"./Images/CollectionTexture/All.png";
	wstring imageFile3 = L"./Images/Chest/Explain.png";
	wstring imageFile4 = L"./Images/Chest/ItemEffect.png";
	wstring imageFile5 = L"./Images/Chest/ItemBox.png";
	wstring imageFile6 = L"./Images/Chest/vfx.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	texture = new Texture(imageFile1, shaderFile);

	item = new Texture(imageFile2, shaderFile);
	item->SetScale(2.45f, 2.45f);
	item->SetOffsetSize(15.0f, 14.0f);

	explain = new Texture(imageFile3, shaderFile);

	itemBG = new Texture(imageFile4, shaderFile);
	itemBG->UpdateColorBuffer(Color(0, 0, 0, 0.75f), 7, 0, 0, 0);
	itemBG->SetScale(1.35f, 1.35f);

	itemBox = new Texture(imageFile5, shaderFile);
	itemBox->SetScale(3.25f, 3.25f);

	vfx = new Texture(imageFile6, shaderFile);

	lvUpMenu = (LvUpMenu*)OBJMANAGER->FindObject("LvUpMenu");

	SetActive(false);
}

ChestEffect::~ChestEffect()
{
	SAFE_DELETE(texture);
}

void ChestEffect::Update(Matrix V, Matrix P)
{
	if (!IsActive())
		return;

	if (time < -1.0f && DOWN(VK_ESCAPE))
	{
		skip = true;
		time = -6.0f;
		Player* pl = PLAYER;
		pl->SetGold(pl->GetGold() + (int)gold);
	}

	if (!skip)
	{
		if (time > -6.0f)
			time -= DELTA;
		else
		{
			skip = true;
			//lvUpMenu->Upgrade(type);
		}
	}

	if (scale < 0.7125f)
		scale += DELTA;

	Vector2 pos = CAMPOS;
	pos.y -= 143.0f;

	Vector2 size = texture->GetTextureRealSize();

	texture->UpdateColorBuffer(Color(25, 25, 255, 0.75f), 7, 0, 0, 0);
	texture->SetScale(7.0f, 7.0f * scale);
	texture->SetPosition(pos.x, pos.y + (size.y / 2));
	texture->Update(V, P);
}

void ChestEffect::Render()
{
	if (!IsActive())
		return;

	GoldRender();

	if (time > 0.0f)
		ItemRoullet();
	else
		Explain();
}

void ChestEffect::Reset()
{
	skip = false;
	ftime = 8.0f;
	time = ftime;
	scale = 0.0f;
	pos = Vector2(-5.0f, -300.0f);

	Player* pl = PLAYER;

	float luck = pl->GetLuck() * 100.0f;

	mt19937	engine((unsigned int)std::time(NULL));
	uniform_int_distribution<> distribution((INT)(10 + luck), (INT)(100 + luck));
	auto generator = bind(distribution, engine);

	gold = (float)generator();
	value = 0.0f;

	cout << "Chest Effect Reset!" << endl;
}


void ChestEffect::GoldRender()
{
	if (value < gold)
		value += (gold / ftime) * DELTA;

	static bool plus = false;
	static float size = 40.0f;

	if (plus)
		size += 5.0f * DELTA;
	else
		size -= 5.0f * DELTA;

	if (size > 45.0f)
		plus = false;
	else if (size < 40.0f)
		plus = true;

	wstring str = to_wstring(time);
	Vector2 pos = Vector2(-300.0f, 0.0f);
	CAMERA->VCToWC(pos);
	//DirectWrite::RenderText(str, pos, 255, 255, 255, 30.0f);

	str = to_wstring(value, L"%.2f");
	pos = Vector2(0.0f, -265.0f);
	pos += CAMPOS;

	if (time > 0.0f)
	{
		pos.x -= str.length() * 20.0f;
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 255, 40.0f);
	}
	else if (time < 0.0f && time > -6.0f)
	{
		pos.x -= str.length() * size / 2;
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 0, size);
	}
	else
	{
		pos.y += 615.0f;
		pos.x -= str.length() * size / 2;
		CAMERA->VCToWC(pos);
		DirectWrite::RenderText(str, pos, 255, 255, 0, size);
	}
}

void ChestEffect::ItemRoullet()
{
	texture->Render();

	pos.y += 800.0f * DELTA;

	if (pos.y >= 100.0f)
	{
		mt19937	engine((unsigned int)std::time(NULL));
		uniform_int_distribution<> distribution(0, 19);
		auto generator = bind(distribution, engine);

		for (int i = 0; i < 7; i++)
			icon[i] = generator();
		pos.y = -200.0f;
	}

	Vector2 bot = Vector2(0.0f, CAMPOS.x - 160.0f);
	Vector2 top = Vector2(0.0f, CAMPOS.x + 310.0f);

	size_t id = 0;

	for (float val = -400.0f; val <= 400.0f; val += 100.0f)
	{
		float size = -item->GetTextureRealSize().y / 2;

		if (pos.y + val + size >= bot.y && pos.y + val + size <= top.y)
		{
			Vector2 offset;
			Vector2 offSize;

			ReadTextFile(icon[id++], offset, offSize);
			
			item->SetOffset(offset);
			item->SetOffsetSize(offSize);
			item->SetPosition(pos.x + CAMPOS.x, pos.y + val + CAMPOS.y);
			item->Update(VM, PM);
			item->Render();
		}
	}
}

void ChestEffect::Explain()
{
	itemBG->SetPosition(CAMPOS.x, CAMPOS.y + 115.0f);
	itemBG->Update(VM, PM);
	itemBG->Render();

	item->SetPosition(CAMPOS.x, CAMPOS.y + 115.0f);
	item->Update(VM, PM);
	item->Render();

	if (!skip)
	{
		if (!confirm)
		{
			ConfirmItem();

			Vector2 offset;
			Vector2 offSize;

			ReadTextFile(type, offset, offSize);

			item->SetOffset(offset);
			item->SetOffsetSize(offSize);

			confirm = true;
		}

		explain->SetPosition(CAMPOS.x, CAMPOS.y - 405.0f);
		explain->Update(VM, PM);
		explain->Render();

		vector<wstring> explains = lvUpMenu->GetExplains();
		
		EquipList* list = (EquipList*)OBJMANAGER->FindObject("EquipList");
		int level = list->GetEquipLevel(type) + 1;

		cout << "아이템 ID : " << type << " 의 레벨 : " << level << endl;

		wstring str = L"레벨 : " + to_wstring(level);
		Vector2 pos = Vector2(CAMPOS.x - 260.0f, CAMPOS.y - 335.0f);
		pos.x -= str.length() * 12.5f;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = explains[type * 10];
		pos = Vector2(CAMPOS.x, CAMPOS.y - 335.0f);
		pos.x -= str.length() * 12.5f;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 0, 25.0f);

		str = explains[type * 10 + level];
		pos = Vector2(CAMPOS.x, CAMPOS.y - 380.0f);
		pos.x -= str.length() * 12.5f;
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);


		itemBox->SetPosition(CAMPOS.x - 260.0f, CAMPOS.y - 420.0f);
		itemBox->Update(VM, PM);
		itemBox->Render();

		item->SetPosition(CAMPOS.x - 260.0f, CAMPOS.y - 420.0f);
		item->Update(VM, PM);
		item->Render();
	}
}

void ChestEffect::ConfirmItem()
{
	Player* pl = PLAYER;

	int* sList = pl->GetEquipedSkillList();
	int* iList = pl->GetEquipedItemList();
	int sAmount = 0;
	int iAmount = 0;


	for (int i = 0; i < 6; i++)
	{
		if (sList[i] != -1)
			sAmount++;

		if (iList[i] != -1)
			iAmount++;
	}

	mt19937	engine((unsigned int)std::time(NULL));

	uniform_int_distribution<> distribution1(0, 1);
	auto generator1 = bind(distribution1, engine);

	if (iAmount > 0 && generator1())
	{
		uniform_int_distribution<> distribution3(0, iAmount - 1);
		auto generator3 = bind(distribution3, engine);

		type = iList[generator3()];
	}
	else
	{
		uniform_int_distribution<> distribution2(0, sAmount - 1);
		auto generator2 = bind(distribution2, engine);

		type = sList[generator2()];
	}
}

void ChestEffect::ReadTextFile(int row, Vector2& offset, Vector2& offsetSize)
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
