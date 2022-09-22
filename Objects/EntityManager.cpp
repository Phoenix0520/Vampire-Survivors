#include "framework.h"
#include "EntityManager.h"
#include "Monster.h"
#include "DropItem.h"
#include "Brazier.h"

EntityManager::EntityManager()
{
	wstring imageFile1 = L"./Images/Status.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	background = new Texture(imageFile1, shaderFile);
}

EntityManager::~EntityManager()
{
	//for (auto objs : mobs)
	//	SAFE_DELETE(objs.second);
	//for (auto objs : items)
	//	SAFE_DELETE(objs.second);
	//for (auto objs : brazs)
	//	SAFE_DELETE(objs.second);
}

void EntityManager::Update(Matrix V, Matrix P)
{
	static bool udt = false;

	if (!udt)
	{
		UpdateAll();
		udt = true;
	}

	background->SetPosition(CAMPOS.x -575.0f, CAMPOS.y -225.0f);
	background->Update(V, P);
}

void EntityManager::Render()
{
	if (!visible)
		return;

	background->Render();
	
	wstring str = L"";

	UINT amount = 0;

	for (UINT i = 0; i < mobs.size(); i++)
	{
		if (mobs[i].second->IsActive())
			amount++;
	}

	str = L"살아있는 몹 : " + to_wstring(amount) + L" 마리";

	float y = 6.0f;

	Vector2 pos = Vector2(CAMPOS.x -575.0f, CAMPOS.y + y);
	pos.x -= str.length() * 7.5f;
	
	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(str, pos, 200, 0, 200, 15.0f);


	UINT index = GetAddableMobIndex();

	str = L"다음 생성될 몹 번호 : " + to_wstring(index);
	if (index == ERR_ID)
		str = L"몹이 최대 용량에 도달했습니다.";

	y -= 20.0f;

	pos = Vector2(CAMPOS.x - 575.0f, CAMPOS.y + y);
	pos.x -= str.length() * 7.5f;

	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(str, pos, 200, 0, 200, 15.0f);


	str = L"---------활성화 된 몹 번호---------\n";

	int col = 0;
	for (UINT i = 0; i < mobs.size(); i++)
	{
		if (mobs[i].second->IsActive())
		{
			if (i < 10)
				str += L"0" + to_wstring(i) + L" ";
			else
				str += to_wstring(i) + L" ";

			col++;

			if (col == 14)
			{
				str += L"\n";
				col = 0;
			}
		}
	}

	y -= 20.0f;

	pos = Vector2(CAMPOS.x - 575.0f, CAMPOS.y + y);
	pos.x -= str.length() * 7.5f;

	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(str, pos, 200, 0, 200, 15.0f);

	///////////////////////////////////////////////////////////////////////

	amount = 0;

	for (UINT i = 0; i < items.size(); i++)
	{
		if (items[i].second->IsActive())
			amount++;
	}

	y -= 120.0f;

	str = L"떨어진 아이템 : " + to_wstring(amount) + L" 개";

	pos = Vector2(CAMPOS.x - 575.0f, CAMPOS.y + y);
	pos.x -= str.length() * 7.5f;

	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(str, pos, 0, 200, 200, 15.0f);

	index = GetAddableItemIndex();

	str = L"다음 생성될 아이템의 번호 : " + to_wstring(index);
	if (index == ERR_ID)
		str = L"아이템이 최대 용량에 도달했습니다.";

	y -= 20.0f;

	pos = Vector2(CAMPOS.x - 575.0f, CAMPOS.y + y);
	pos.x -= str.length() * 7.5f;

	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(str, pos, 0, 200, 200, 15.0f);

	str = L"---------활성화 된 아이템 번호---------\n";

	col = 0;
	for (UINT i = 0; i < items.size(); i++)
	{
		if (items[i].second->IsActive())
		{
			if (i < 10)
				str += L"0" + to_wstring(i) + L" ";
			else
				str += to_wstring(i) + L" ";

			col++;

			if (col == 14)
			{
				str += L"\n";
				col = 0;
			}
		}
	}

	y -= 20.0f;

	pos = Vector2(CAMPOS.x - 575.0f, CAMPOS.y + y);
	pos.x -= str.length() * 7.5f;

	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(str, pos, 0, 200, 200, 15.0f);

	///////////////////////////////////////////////////////////////

	amount = 0;

	for (UINT i = 0; i < brazs.size(); i++)
	{
		if (brazs[i].second->IsActive())
			amount++;
	}

	str = L"생성된 화로 : " + to_wstring(amount) + L" 개";

	y -= 120.0f;

	pos = Vector2(CAMPOS.x - 575.0f, CAMPOS.y + y);
	pos.x -= str.length() * 7.5f;

	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(str, pos, 200, 200, 0, 15.0f);

	index = GetAddableBrazIndex();

	str = L"다음 생성될 화로의 번호 : " + to_wstring(index);
	if (index == ERR_ID)
		str = L"화로가 최대 용량에 도달했습니다.";

	y -= 20.0f;

	pos = Vector2(CAMPOS.x - 575.0f, CAMPOS.y + y);
	pos.x -= str.length() * 7.5f;

	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(str, pos, 200, 200, 0, 15.0f);

	str = L"---------활성화 된 화로 번호---------\n";

	col = 0;
	for (UINT i = 0; i < brazs.size(); i++)
	{
		if (brazs[i].second->IsActive())
		{
			if (i < 10)
				str += L"0" + to_wstring(i) + L" ";
			else
				str += to_wstring(i) + L" ";

			col++;

			if (col == 14)
			{
				str += L"\n";
				col = 0;
			}
		}
	}

	y -= 20.0f;

	pos = Vector2(CAMPOS.x - 575.0f, CAMPOS.y + y);
	pos.x -= str.length() * 7.5f;

	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(str, pos, 200, 200, 0, 15.0f);
}

void EntityManager::UpdateAll()
{
	UpdateMob();
	UpdateItem();
	UpdateBrazier();
}

void EntityManager::UpdateMob()
{
	mobs.clear();

	for (UINT i = 0; i < MAX_MOB; i++)
	{
		string str = "Monster" + to_string(i);

		Monster* obj = (Monster*)OBJMANAGER->FindObject(str);

		//if (obj->IsActive())
		if (obj)
			mobs.push_back(make_pair(str, obj));
	}
}

void EntityManager::UpdateItem()
{
	items.clear();

	for (UINT i = 0; i < MAX_ITEM; i++)
	{
		string str = "DropItem" + to_string(i);

		DropItem* obj = (DropItem*)OBJMANAGER->FindObject(str);

		if (obj)
			items.push_back(make_pair(str, obj));
	}
}

void EntityManager::UpdateBrazier()
{
	brazs.clear();

	for (UINT i = 0; i < MAX_ITEM; i++)
	{
		string str = "Brazier" + to_string(i);

		Brazier* obj = (Brazier*)OBJMANAGER->FindObject(str);

		if (obj)
			brazs.push_back(make_pair(str, obj));
	}
}

vector<Monster*> EntityManager::GetAvailMobs()
{
	vector<Monster*> temp;

	for (UINT i = 0; i < mobs.size(); i++)
	{
		temp.push_back(mobs[i].second);
	}

	return temp;
}

vector<DropItem*> EntityManager::GetAvailItems()
{
	vector<DropItem*> temp;

	for (UINT i = 0; i < items.size(); i++)
	{
		temp.push_back(items[i].second);
	}

	return temp;
}

vector<Brazier*> EntityManager::GetAvailBraziers()
{
	vector<Brazier*> temp;

	for (UINT i = 0; i < brazs.size(); i++)
	{
		temp.push_back(brazs[i].second);
	}

	return temp;
}

UINT EntityManager::GetAddableMobIndex()
{
	for (UINT i = 0; i < MAX_MOB; i++)
	{
		Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));

		if (!mob)
			return ERR_ID;
		if (!mob->IsActive())
			return i;
	}

	return ERR_ID;
}

UINT EntityManager::GetAddableItemIndex()
{
	for (UINT i = 0; i < MAX_ITEM; i++)
	{
		DropItem* item = (DropItem*)OBJMANAGER->FindObject("DropItem" + to_string(i));

		if (!item)
			return ERR_ID;
		if (!item->IsActive())
			return i;
	}

	return ERR_ID;
}

UINT EntityManager::GetAddableBrazIndex()
{
	for (UINT i = 0; i < MAX_MOB; i++)
	{
		Brazier* obj = (Brazier*)OBJMANAGER->FindObject("Brazier" + to_string(i));

		if (!obj)
			return ERR_ID;
		if (!obj->IsActive())
			return i;
	}

	return ERR_ID;
}

UINT EntityManager::GetMobIndex(Monster* mob)
{
	for (UINT i = 0; i < mobs.size(); i++)
	{
		if (mob == mobs[i].second)
			return i;
	}

	return ERR_ID;
}

UINT EntityManager::GetItemIndex(DropItem* item)
{
	for (UINT i = 0; i < items.size(); i++)
	{
		if (item == items[i].second)
			return i;
	}

	return ERR_ID;
}

UINT EntityManager::GetBrazIndex(Brazier* braz)
{
	for (UINT i = 0; i < brazs.size(); i++)
	{
		if (braz == brazs[i].second)
			return i;
	}

	return ERR_ID;
}

void EntityManager::RemoveMob(UINT index)
{
	string id = "Monster" + to_string(index);

	Monster* obj = (Monster*)OBJMANAGER->FindObject(id);

	if (!obj->IsActive())
		return;

	vector<pair<string, Monster*>> temp = mobs;

	mobs.clear();

	for (UINT i = 0; i < temp.size(); i++)
	{
		if (temp[i].first == id)
			continue;
		else
			mobs.push_back(make_pair(temp[i].first, temp[i].second));
	}
}

void EntityManager::RemoveItem(UINT index)
{
	string id = "DropItem" + to_string(index);

	DropItem* obj = (DropItem*)OBJMANAGER->FindObject(id);

	if (!obj->IsActive())
		return;

	vector<pair<string, DropItem*>> temp = items;

	items.clear();

	for (UINT i = 0; i < temp.size(); i++)
	{
		if (temp[i].first == id)
			continue;
		else
			items.push_back(make_pair(temp[i].first, temp[i].second));
	}
}

void EntityManager::RemoveBrzier(UINT index)
{
	string id = "Brazier" + to_string(index);

	Brazier* obj = (Brazier*)OBJMANAGER->FindObject(id);

	if (!obj->IsActive())
		return;

	vector<pair<string, Brazier*>> temp = brazs;

	brazs.clear();

	for (UINT i = 0; i < temp.size(); i++)
	{
		if (temp[i].first == id)
			continue;
		else
			brazs.push_back(make_pair(temp[i].first, temp[i].second));
	}
}
