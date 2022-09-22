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
}

void SkillEffect::Update(Matrix V, Matrix P)
{
	coolTime = sl->GetSkillCoolTime(id);
	coolTime -= coolTime * pl->GetCoolDown();
	
	area = sl->GetSkillArea(id);
	area += 0.01f * pl->GetArea();

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
}

//void SkillEffect::UpdateSkillEffect1(Matrix V, Matrix P)
//{
//	texture->SetOffset(0.0f, 0.0f);
//	texture->SetOffsetSize(240.0f, 53.0f);
//
//	time += DELTA;
//
//	Vector2 pos = pl->GetPosition();
//
//	if (time >= coolTime)
//	{
//		for (UINT i = 0; i < MAX_MOB; i++)
//			attackedID[0][i] = false;	
//
//		texture->SetRotation(GetRotation());
//	
//		pos = pl->GetPosition();
//
//		if (GetRotation().y == 0.0f)
//			texture->SetPosition(pos.x + 140.0f, pos.y + 2.5f);
//		else
//			texture->SetPosition(pos.x - 140.0f, pos.y + 2.5f);
//
//		time = 0.0f;
//
//		mob.clear();
//
//		for (int i = 0; i < MAX_MOB; i++)
//		{
//			Monster* mons = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));
//			mob.push_back(mons);
//		}
//	}
//
//	float alpha = time / coolTime + 0.5f;
//
//	texture->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, alpha * 1.5f), 7, 0, 0, 0);
//
//	float scale = 1.0f + 1.0f * area;
//
//	if (time <= 0.15f)
//		texture->SetScale(time * 10.0f * scale, time * 10.0f * scale);
//	else if (time > 0.15f && time <= 0.2f)
//		texture->SetScale((time - 0.15f) * 1.5f, (time - 0.15f) * 1.5f);
//
//	if (texture->GetScale().x < 0.1f)
//		texture->SetScale(0.0f, 0.0f);
//
//	for (UINT i = 0; i < MAX_MOB; i++)
//	{
//		mob[i] = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));
//
//		if (IsMonsterInRegion(mob[i]))
//		{
//			if (!attackedID[0][i])
//				attackedID[0][i] = true;
//			else
//				continue;
//
//			PLAYSOUND("Hit", sfxSize);
//			mob[i]->Attacked(id);
//		}
//	}
//
//	texture->Update(V, P);
//}
//
//void SkillEffect::UpdateSkillEffect2(Matrix V, Matrix P)
//{
//	time += DELTA;
//
//	static float tTime = 0.0f;
//	static bool toggle = false;
//
//	if (tTime >= 0.1f)
//	{
//		if (toggle)
//			toggle = false;
//		else
//			toggle = true;
//
//		tTime = 0.0f;
//	}
//	else
//	{
//		tTime += DELTA;
//	}
//
//	if (toggle)
//	{
//		texture->SetOffset(252.0f, 0.0f);
//		texture->SetOffsetSize(14.0f, 25.0f);
//	}
//	else
//	{
//		texture->SetOffset(240.0f, 0.0f);
//		texture->SetOffsetSize(12.0f, 25.0f);
//	}
//
//	float scale = 1.5f + 1.5f * area;
//	texture->SetScale(scale, scale);
//	// 스킬 범위
//
//	float mSpeed = 500.0f + 500.0f * speed;
//	// 투사체 속도
//
//	Vector2 mPos; 
//	// 몬스터 위치
//
//	static Vector2 pos1;
//	static Vector2 pos2;
//	// 몬스터 ID
//
//	if (time >= coolTime)
//	{
//		soundPlayed[1] = false;
//
//		for (UINT i = 0; i < MAX_MOB; i++)
//			attackedID[1][i] = false;
//
//		time = 0.0f;
//		pos2 = pl->GetPosition();
//		interval = 100000.0f;
//
//		mob.clear();
//
//		for (int i = 0; i < MAX_MOB; i++)
//		{
//			Monster* mons = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));
//			mob.push_back(mons);
//		}
//
//		SetActive(true);
//	}
//
//	if (!IsActive())
//		return;
//
//	for (int i = 0; i < MAX_MOB; i++)
//	{
//		if (!attackedID[1][i])
//			attackedID[1][i] = true;	
//		else
//			continue;
//
//		if (IsMonsterInRegion(mob[i], 0.0f, 1.0f))
//		{
//			mob[i]->Attacked(id);
//			pos2 = pl->GetPosition();
//			interval = 100000.0f;
//			SetActive(false);
//			return;
//		}
//
//		mPos = mob[i]->GetPosition();
//
//		float x = fabsf(mPos.x - pos2.x);
//		float y = fabsf(mPos.y - pos2.y);
//
//		float itv = sqrtf(x * x + y * y);
//
//		if (itv < interval)
//		{
//			pos1 = mPos;
//			interval = itv;
//		}
//	}
//
//	float angle = atan2((pos1.y - pos2.y), (pos1.x - pos2.x));
//
//	pos2.x += cosf(angle) * DELTA * mSpeed;
//	pos2.y += sinf(angle) * DELTA * mSpeed;
//
//	float rot = atan2((pos1.y - pl->GetPosition().y), (pos1.x - pl->GetPosition().x));
//
//	rot = (((-rot + (float)D3DX_PI) / (float)D3DX_PI) * 180.0f) + 90.0f;
//	if (rot >= 360.0f)
//		rot -= 360.0f;
//
//	texture->SetRotation(0.0f, 180.0f, rot);
//	texture->SetPosition(pos2);
//	texture->Update(V, P);
//}
//
//void SkillEffect::UpdateSkillEffect3(Matrix V, Matrix P)
//{
//	static float tTime = 0.0f;
//	static bool toggle = false;
//
//	if (tTime >= 0.1f)
//	{
//		if (toggle)
//			toggle = false;
//		else
//			toggle = true;
//
//		tTime = 0.0f;
//	}
//	else
//	{
//		tTime += DELTA;
//	}
//
//	if (toggle)
//	{
//		texture->SetOffset(271.0f, 0.0f);
//		texture->SetOffsetSize(6.0f, 35.0f);
//	}
//	else
//	{
//		texture->SetOffset(265.0f, 0.0f);
//		texture->SetOffsetSize(6.0f, 30.0f);
//	}
//
//	time += DELTA;
//	
//
//	Vector3 rot = pl->GetRotation();
//	static Vector2 mPos = Vector2(1.0f, 0.0f);
//	static Vector2 oldmPos = mPos;
//	static Vector2 pos = texture->GetPosition();
//	static float oldAngle = 90.0f;
//
//	if (time >= coolTime)
//	{
//		soundPlayed[2] = false;
//
//		for (UINT i = 0; i < MAX_MOB; i++)
//			attackedID[2][i] = false;
//
//		texture->SetPosition(pl->GetPosition());
//		time = 0.0f;
//
//		mPos = pl->GetMovePos();
//		pos = texture->GetPosition();
//
//		mob.clear();
//
//		for (int i = 0; i < MAX_MOB; i++)
//		{
//			Monster* mons = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));
//			mob.push_back(mons);
//		}
//
//		SetActive(true);
//	}
//
//	if (!IsActive())
//		return;
//
//	if (mPos.x == 0 && mPos.y == 1)
//		rot.z = 0.0f;
//	else if (mPos.x == 1 && mPos.y == 1)
//		rot.z = 45.0f;
//	else if (mPos.x == 1 && mPos.y == 0)
//		rot.z = 90.0f;
//	else if (mPos.x == 1 && mPos.y == -1)
//		rot.z = 135.0f;
//	else if (mPos.x == 0 && mPos.y == -1)
//		rot.z = 180.0f;
//	else if (mPos.x == -1 && mPos.y == -1)
//		rot.z = 225.0f;
//	else if (mPos.x == -1 && mPos.y == 0)
//		rot.z = 270.0f;
//	else if (mPos.x == -1 && mPos.y == 1)
//		rot.z = 315.0f;
//
//	if (mPos.x == 0 && mPos.y == 0)
//	{
//		rot.z = oldAngle;
//		mPos = pl->GetOldMovePos();
//		texture->SetRotation(0.0f, 180.0f, rot.z);
//	}
//	else
//	{
//		oldAngle = rot.z;
//		oldmPos = mPos;
//		texture->SetRotation(0.0f, 180.0f, rot.z);
//	}
//
//	// 방향
//
//	if (mPos.x != 0.0f && mPos.y != 0.0f)
//	{
//		pos.x += (mPos.x * (15.0f * (1.0f + speed))) / sqrtf(2.0f);
//		pos.y += (mPos.y * (15.0f * (1.0f + speed))) / sqrtf(2.0f);
//	}
//	else
//	{
//		pos.x += mPos.x * (15.0f * (1.0f + speed));
//		pos.y += mPos.y * (15.0f * (1.0f + speed));
//	}
//	
//	texture->SetPosition(pos);
//	// 속도 및 이동
//
//	float scale = 2.25f + 2.25f * area;
//
//	texture->SetScale(scale, scale);
//
//	// 스킬 범위
//	
//	for (int i = 0; i < MAX_MOB; i++)
//	{
//		if (IsMonsterInRegion(mob[i], 17.5f, 17.5f))
//		{
//			mob[i]->Attacked(id);
//			spearCount++;
//
//			if (spearCount >= spear)
//			{
//				spearCount = 0;
//				SetActive(false);
//				return;
//			}
//		}
//	}
//
//	texture->Update(V, P);
//}
//
//void SkillEffect::UpdateSkillEffect4(Matrix V, Matrix P)
//{
//	texture->SetOffset(277.0f, 0.0f);
//	texture->SetOffsetSize(16.0f, 18.0f);
//
//	time += DELTA;
//
//	static Vector2 pos = pl->GetPosition();
//
//	static float gravity = 0.0f;
//	static float angle = 0.0f;
//
//	if (time >= coolTime)
//	{
//		for (UINT i = 0; i < MAX_MOB; i++)
//			attackedID[3][i] = false;
//
//		pos = pl->GetPosition();
//		gravity = 0.0f;
//		angle = 0.0f;
//		time = 0.0f;
//
//		mob.clear();
//
//		for (int i = 0; i < MAX_MOB; i++)
//		{
//			Monster* mons = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));
//			mob.push_back(mons);
//		}
//
//		SetActive(true);
//	}
//
//	if (!IsActive())
//		return;
//
//
//	if (angle == 0.0f)
//		pos.x += 0.0f;
//	else if (angle > 0.0f)
//		pos.x += cosf(angle) * DELTA * 350.0f;
//	else
//		pos.x -= cosf(angle) * DELTA * 350.0f;
//
//	pos.y += sinf(0.975f) * 1000.0f * DELTA - gravity;
//
//	gravity += 0.45f + 0.45f * speed;
//
//	texture->SetPosition(pos);
//
//	// 속도 및 이동
//
//	static float rot = 0.0f;
//	rot = (time * 0.5f) * 540.0f;
//
//	texture->SetRotation(0.0f, 180.0f, rot);
//
//	// 회전 설정
//
//	float scale = 2.75f + 2.75f * area;
//
//	texture->SetScale(scale, scale);
//	// 크기 설정
//
//	for (int i = 0; i < MAX_MOB; i++)
//	{
//		if (IsMonsterInRegion(mob[i]))
//		{
//			if (!attackedID[3][i])
//				attackedID[3][i] = true;
//			else
//				continue;
//
//			mob[i]->Attacked(id);
//			spearCount++;
//
//			if (spearCount >= spear)
//			{
//				spearCount = 0;
//				SetActive(false);
//				return;
//			}
//		}
//	}
//
//	texture->Update(V, P);
//}
//
//void SkillEffect::UpdateSkillEffect5(Matrix V, Matrix P)
//{
//	texture->SetOffset(293.0f, 0.0f);
//	texture->SetOffsetSize(14.0f, 14.0f);
//
//	time += DELTA;
//
//	static Vector2 mPos;
//	static Vector2 pos1;
//	static Vector2 pos2;
//	static float angle = 0.0f;
//
//	if (time >= coolTime)
//	{
//		for (UINT i = 0; i < MAX_MOB; i++)
//			attackedID[4][i] = false;
//
//		time = 0.0f;
//		mPos = Vector2(0.0f, 0.0f);
//		pos1 = Vector2(0.0f, 0.0f);
//		pos2 = pl->GetPosition();
//
//		interval = 100000.0f;
//
//		mob.clear();
//
//		for (int i = 0; i < MAX_MOB; i++)
//		{
//			Monster* mons = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));
//			mob.push_back(mons);
//		}
//
//		SetActive(true);
//	}
//
//	if (!IsActive())
//		return;
//
//	float rot = 0.0f;
//
//	float mSpeed = 400.0f + 400.0f * speed;
//	// 투사체 속도
//
//	if (time <= 0.35f)
//	{
//		pos2.x += cosf(angle) * DELTA * (-time * 2.0f + 1.5f) * mSpeed;
//		pos2.y += sinf(angle) * DELTA * (-time * 2.0f + 1.5f) * mSpeed;
//
//		rot = time * 540.0f;
//	}
//	else
//	{
//		pos2.x -= cosf(angle) * DELTA * (time + 0.75f) * mSpeed;
//		pos2.y -= sinf(angle) * DELTA * (time + 0.75f) * mSpeed;
//
//		rot = time * 720.0f;
//	}
//
//
//	float scale = 2.0f + 2.0f * area;
//	texture->SetScale(scale, scale);
//	// 스킬 범위
//	texture->SetPosition(pos2);
//	// 이동 및 속도
//
//	//angle = time * 540.0f;
//
//	texture->SetRotation(0.0f, 180.0f, rot);
//	// 회전
//
//	texture->Update(V, P);
//
//	for (int i = 0; i < MAX_MOB; i++)
//	{
//		if (IsMonsterInRegion(mob[i]))
//		{
//			if (!attackedID[4][i])
//				attackedID[4][i] = true;
//			else
//				continue;
//
//			mob[i]->Attacked(id);
//		}
//	}
//}
//
//void SkillEffect::UpdateSkillEffect6(Matrix V, Matrix P)
//{
//	texture->SetOffset(307.0f, 0.0f);
//	texture->SetOffsetSize(10.0f, 15.0f);
//
//	time += DELTA;
//
//	Vector2 pos = pl->GetPosition();
//	static float angle = 135.0f;
//
//	if (time >= coolTime)
//	{
//		for (UINT i = 0; i < MAX_MOB; i++)
//			attackedID[5][i] = false;
//
//		time = 0.0f;
//		angle = 135.0f;
//
//		mob.clear();
//
//		for (int i = 0; i < MAX_MOB; i++)
//		{
//			Monster* mons = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));
//			mob.push_back(mons);
//		}
//
//		SetActive(true);
//	}
//	
//	if (time >= duration)
//	{
//		SetActive(false);
//	}
//
//	if (!IsActive())
//		return;
//	
//	float x1 = cosf((float)D3DXToRadian(angle)) * (125.0f * (1 + (area / 2)));
//	float y1 = sinf((float)D3DXToRadian(angle)) * (125.0f * (1 + (area / 2)));
//	Vector2 start = Vector2(x1, y1);
//	pos -= start;
//	texture->SetPosition(pos);
//	pos += start;
//	// 위치 설정
//
//	float mSpeed = (360.0f / duration) * (1 + speed);
//	angle += mSpeed * DELTA;
//	// 이동속도
//
//	float scale = 2.5f + 2.5f * area;
//
//	if (time <= 0.5f)
//		scale *= (time + 0.5f);
//	else if (time >= duration - 0.5f)
//		scale *= (-time + duration + 0.5f);
//	else
//		scale = 2.5f + 2.5f * area;
//
//	texture->SetScale(scale, scale);
//	// 크기 설정
//
//	texture->SetRotation(0.0f, 0.0f, 0.0f);
//	// 각도 변화 없음
//
//	texture->Update(V, P);
//
//	for (int i = 0; i < MAX_MOB; i++)
//	{
//		if (IsMonsterInRegion(mob[i]))
//		{
//			mob[i]->Attacked(id);
//		}
//	}
//}
//
//void SkillEffect::UpdateSkillEffect7(Matrix V, Matrix P)
//{
//	mt19937	engine((unsigned int)std::time(NULL));
//	uniform_int_distribution<> distribution(0, 360);
//	auto generator = bind(distribution, engine);
//
//	static Vector2 pos = pl->GetPosition();
//	static float angle = (float)generator();
//	static bool toggle = false;
//	static float tTime = 0.0f;
//	
//	if (toggle)
//	{
//		texture->SetOffset(331.0f, 0.0f);
//		texture->SetOffsetSize(14.0f, 29.0f);
//	}
//	else
//	{
//		texture->SetOffset(345.0f, 0.0f);
//		texture->SetOffsetSize(14.0f, 29.0f);
//	}
//
//	if (tTime >= 0.1f)
//	{
//		if (toggle)
//			toggle = false;
//		else
//			toggle = true;
//
//		tTime = 0.0f;
//	}
//	else
//	{
//		tTime += DELTA;
//	}
//
//
//	time += DELTA;
//
//	if (time >= coolTime)
//	{
//		for (UINT i = 0; i < MAX_MOB; i++)
//			attackedID[6][i] = false;
//
//		time = 0.0f;
//		pos = pl->GetPosition();
//		angle = (float)generator();
//
//		mob.clear();
//
//		for (int i = 0; i < MAX_MOB; i++)
//		{
//			Monster* mons = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));
//			mob.push_back(mons);
//		}
//
//		SetActive(true);
//	}
//
//	if (!IsActive())
//		return;
//
//	pos.x += cosf((float)D3DXToRadian(angle)) * DELTA * (150.0f * (1 + speed));
//	pos.y += sinf((float)D3DXToRadian(angle)) * DELTA * (150.0f * (1 + speed));
//	
//	texture->SetPosition(pos);
//	// 이동 및 속도
//
//	texture->SetRotation(0.0f, 0.0f, angle - 90.0f);
//	// 회전
//
//	float scale = 2.5f + (2.5f * area);
//
//	texture->SetScale(scale, scale);
//	// 크기
//
//	for (UINT i = 0; i < ids.size(); i++)
//	{
//		if (IsMonsterInRegion(mob[i]))
//		{
//			if (!attackedID[6][i])
//				attackedID[6][i] = true;
//			else
//				continue;
//
//			mob[i]->Attacked(id);
//			SetActive(false);
//			return;
//		}
//	}
//
//	texture->Update(V, P);
//}
//
//void SkillEffect::UpdateSkillEffect8(Matrix V, Matrix P)
//{
//	texture->SetOffset(360.0f, 0.0f);
//	texture->SetOffsetSize(157.0f, 157.0f);
//	
//	time += DELTA;
//
//	if (time >= coolTime)
//	{
//		for (UINT i = 0; i < MAX_MOB; i++)
//			attackedID[7][i] = false;
//
//		time = 0.0f;
//	}
//
//	static bool plus = true;
//	static float ctime = 0.0f;
//
//	if (plus)
//		ctime += DELTA * 0.25f;
//	else
//		ctime -= DELTA * 0.25f;
//
//	if (ctime >= 0.4f)
//		plus = false;
//	if (ctime <= 0.1f)
//		plus = true;
//
//	float alpha = ctime;
//
//	texture->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, alpha), 7, 0, 0, 0);
//
//	static float rot = 0.0f;
//	rot += DELTA * 120.0f;
//	if (rot >= 360.0f)
//		rot -= 360.0f;
//
//	texture->SetRotation(0.0f, 0.0f, rot);
//
//	float scale = 0.85f + 0.85f * area;
//
//	texture->SetScale(scale, scale);
//
//	Vector2 pos = pl->GetPosition();
//	pos.y -= 12.5f;
//
//	texture->SetPosition(pos);
//
//	texture->Update(V, P);
//
//	for (int i = 0; i < MAX_MOB; i++)
//	{
//		if (!attackedID[7][i])
//			attackedID[7][i] = true;
//		else
//			continue;
//
//		if (IsMonsterInRegion(mob[i]))
//		{
//			mob[i]->Attacked(id);
//			return;
//		}
//	}
//}

bool SkillEffect::IsMonsterInRegion(class Monster* mob, float advX, float advY)
{
	if (mob->GetState() == Monster::DEAD)
		return false;
	
	if (!mob->GetIntersect())
		return false;
	 
	Vector2 tPos = texture->GetPosition();
	Vector2 mPos = mob->GetPosition();
	Vector2 size = texture->GetTextureRealSize() / 2;

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
