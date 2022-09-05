#include "framework.h"
#include "MobManager.h"
#include "Monster.h"

MobManager::MobManager()
{
	Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster0");
	mobs.insert(make_pair("Bat", mob));

	//Vector2 pos = Vector2(250.0f, 250.0f);
	//mobPos.push_back(pos);
}

MobManager::~MobManager()
{
	for (auto a : mobs)
		SAFE_DELETE(a.second);
}

void MobManager::Update(string name, Matrix V, Matrix P)
{
	Monster* mob = FindMob(name);
	if (mob == nullptr)
		return;

	mob->Update(V, P);
}

void MobManager::Render(string name)
{
	Monster* mob = FindMob(name);
	if (mob == nullptr)
		return;

	for (UINT i = 0; i < mobPos.size(); i++)
	{
		mob->SetPosition(mobPos[i]);
		mob->Update(VM, PM);
		mob->Render();
	}

}

void MobManager::AddMob(string name, class Monster * mob)
{
	mobs.insert(make_pair(name, mob));
}

void MobManager::AddPosition(Vector2 pos)
{
	mobPos.push_back(pos);
}

void MobManager::AddPosition(float x, float y)
{
	mobPos.push_back(Vector2(x, y));
}

void MobManager::SetPosition(int ID, Vector2 pos)
{
	if (mobPos.size() <= (UINT)ID)
		return;
	

	mobPos[ID] = pos;
}

void MobManager::SetPosition(int ID, float x, float y)
{
	if (mobPos.size() <= (UINT)ID)
		return;

	mobPos[ID] = Vector2(x, y);
}

Monster* MobManager::FindMob(string name)
{
	Monster* mob = nullptr;

	if (mobs.count(name))
		mob = mobs[name];

	return mob;
}
