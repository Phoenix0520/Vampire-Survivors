#include "framework.h"
#include "SkillEffect.h"
#include "EquipList.h"
#include "Player.h"
#include "Monster.h"
#include "Brazier.h"
#include "DropItem.h"

SkillEffect::SkillEffect()
{
	wstring imageFile1 = L"./Images/SkillEffect.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	texture = new Texture(imageFile1, shaderFile);
	collider = new Collider();
}

SkillEffect::~SkillEffect()
{
	SAFE_DELETE(texture);
	SAFE_DELETE(collider);
}

void SkillEffect::Update(Matrix V, Matrix P)
{
	coolTime = sl->GetSkillCoolTime(id);
	coolTime -= coolTime * pl->GetCoolDown();
	
	area = 0.0f;
	area += pl->GetArea();
	area += sl->GetSkillArea(id);

	speed = sl->GetSkillSpeed(id);
	speed += speed * pl->GetSkillSpeed();

	duration = sl->GetSkillDuration(id);
	duration += duration * pl->GetDuration();

	spear = sl->GetSkillSpear(id);

	damage = sl->GetSkillDamage(id);
	damage += damage * pl->GetPower();

	ids = pl->GetMobsInScreen();

	//switch (id)
	//{
	//case 0:
	//{
	//	UpdateSkillEffect1(V, P);
	//	break;
	//}
	//case 1:
	//{
	//	if (!soundPlayed[1])
	//	{
	//		PLAYSOUND("MagicWand", sfxSize);
	//		soundPlayed[1] = true;
	//	}
	//
	//	UpdateSkillEffect2(V, P);
	//	break;
	//}
	//case 2:
	//{
	//	if (!soundPlayed[2])
	//	{
	//		PLAYSOUND("Projectile", sfxSize);
	//		soundPlayed[2] = true;
	//	}
	//	UpdateSkillEffect3(V, P);
	//	break;
	//}
	//case 3:
	//{
	//	UpdateSkillEffect4(V, P);
	//	break;
	//}
	//case 4:
	//{
	//	UpdateSkillEffect5(V, P);
	//	break;
	//}
	//case 5:
	//{
	//	UpdateSkillEffect6(V, P);
	//	break;
	//}
	//case 6:
	//{
	//	UpdateSkillEffect7(V, P);
	//	break;
	//}
	//case 7:
	//{
	//	UpdateSkillEffect8(V, P);
	//	break;
	//}
	//default:
	//{
	//	break;
	//}
	//}

	collider->SetPosition(texture->GetPosition());
	collider->SetRotation(texture->GetRotation());
	collider->SetScale(texture->GetTextureRealSize());
	collider->Update(V, P);

	for (UINT i = 0; i < brzs.size(); i++)
	{
		if (IsBrazierInRegion(brzs[i]))
		{
			brzs[i]->Attacked(damage);
		}
	}
}

void SkillEffect::Render()
{
	if (!IsActive())
		return;

	//texture->Render();
	collider->Render();
}

void SkillEffect::Reset()
{
	pl = PLAYER;
	sl = (EquipList*)OBJMANAGER->FindObject("EquipList");

	ENTMANAGER->UpdateBrazier();
	brzs = ENTMANAGER->GetAvailBraziers();

	coolTime = sl->GetSkillCoolTime(id);
	coolTime -= coolTime * pl->GetCoolDown();

	time = coolTime;
}

bool SkillEffect::IsMonsterInRegion(class Monster* mob, float advX, float advY)
{
	if (mob->GetState() == Monster::DEAD)
		return false;
	
	if (!mob->GetIntersect())
		return false;
	 
	Vector2 tPos = texture->GetPosition();
	Vector2 mPos = mob->GetPosition();
	Vector2 size = texture->GetTextureRealSize() / 2 + Vector2(advX, advY);

	if (mPos.x > tPos.x - size.x && mPos.x < tPos.x + size.x &&
		mPos.y > tPos.y - size.y && mPos.y < tPos.y + size.y)
	{
		return true;
	}

	return false;
}

bool SkillEffect::IsBrazierInRegion(Brazier * obj, float advX, float advY)
{
	if (!obj->IsActive())
		return false;

	Vector2 tPos = texture->GetPosition();
	Vector2 mPos = obj->GetPosition();
	Vector2 size = texture->GetTextureRealSize() + Vector2(advX, advY);

	if (mPos.x > tPos.x - size.x / 2 && mPos.x < tPos.x + size.x / 2 &&
		mPos.y > tPos.y - size.y / 2 && mPos.y < tPos.y + size.y / 2)
	{
		return true;
	}

	return false;
}
