#include "framework.h"
#include "Player.h"
#include "ObjectList.h"
#include "HitEffect.h"
#include "ScreenEffect.h"
#include "FireEffect.h"
#include "ChestMenu.h"
#include "Whip.h"

#define LvLock	false

Player::Player()
{
	wstring imageFile1 = L"./Images/Player.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	animation = new Animation(imageFile1, shaderFile);

	// Set Clip
	{
		AnimationClip* clip = new AnimationClip(AnimationClip::State::loop);
		animation->AddClip(clip);

		clip->AddFrame(animation->GetTexture(), imageFile1, 0.0f,  0.0f, 28.0f, 32.0f, 0.125f);
		clip->AddFrame(animation->GetTexture(), imageFile1, 29.0f, 0.0f, 58.0f, 32.0f, 0.125f);
		clip->AddFrame(animation->GetTexture(), imageFile1, 59.0f, 0.0f, 89.0f, 32.0f, 0.125f);
		clip->AddFrame(animation->GetTexture(), imageFile1, 91.0f, 0.0f, 119.0f, 32.0f, 0.125f);
	}

	animation->SetScale(2.25f, 2.275f);

	wstring imageFile2 = L"./Images/InGameSceneTexture/HpBar.png";
	wstring imageFile3 = L"./Images/InGameSceneTexture/HpBar2.png";

	hpBar[0] = new Texture(imageFile2, shaderFile);
	hpBar[1] = new Texture(imageFile3, shaderFile);
	
	bloodEffect[1] = new BloodEffect();
	screenEffect = new ScreenEffect();
	fireEffect = new FireEffect();

	collider = new Collider();

	SetActive(false);

	//Reset();
}

Player::~Player()
{
	SAFE_DELETE(animation);
	SAFE_DELETE(hpBar[0]);
	SAFE_DELETE(hpBar[1]);
	SAFE_DELETE(bloodEffect[1]);
	SAFE_DELETE(screenEffect);
	SAFE_DELETE(fireEffect);
	SAFE_DELETE(collider);
}

void Player::Update(Matrix V, Matrix P)
{
	if (!IsActive())
	{
		return;
	}

	SetMobsInScreen();

	skills = (EquipList*)OBJMANAGER->FindObject("EquipList");

	if (SCENEMANAGER->GetCurScene() != SCENEMANAGER->GetScene("InGameScene"))
	{
		SetActive(false);
		return;
	}

	if (DOWN('1'))
	{
		area += 0.1f;
	}

	if (DOWN('Z'))
	{
		luck += 0.1f;
	}

	if (DOWN('X'))
	{
		AddExp(10.0f);
	}

	if (DOWN('C'))
	{
		AddExp(100.0f);
	}

	if (DOWN('F'))
	{
		UseChest();
	}

	// Time
	{
		time += DELTA;

		if (time >= 1.0f)
		{
			if (hp < maxHp)
			{
				hp += recovery;
			}
			time = 0.0f;
		}
	}
	
	// HP
	{
		// Die
		if (hp <= 0)
		{
			animation->UpdateColorBuffer(Color(1.0f, 0.1f, 0.1f, 1.0f), 4, 0, 0, 0);
			return;
		}

		// UnderAttack
		//for (UINT i = 0; i < mobsInScreen.size(); i++)
		//{
		//	Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(mobsInScreen[i]));
		//
		//	if (!mob->IsActive())
		//		continue;
		//
		//	if (IsMonsterInRegion(mob))
		//	{
		//		if (!mob->GetIntersect())
		//			continue;
		//
		//		if (mob->GetDamage() - def > 0.0f)
		//		{
		//			cout << mobsInScreen[i]  << " 번째 몹에게 공격받고 있습니다." << endl;
		//			hp -= (1.0f / 60.0f) * (mob->GetDamage() - def);
		//		}
		//
		//		ctime = 0.0f;
		//		
		//		state = ATTACKED;
		//	}
		//}

		if (state == ATTACKED)
		{
			bloodEffect[1]->Update(V, P);

			animation->UpdateColorBuffer(Color(1.0f, 0.0f, 0.0f, ctime), 12, 0, 0, 0);

			ctime += DELTA;

			if (ctime >= 0.35f)
			{
				bloodEffect[1]->Reset();

				ctime = 0.0f;
				state = IDLE;
			}
		}
		else
			animation->UpdateColorBuffer(Color(), 0, 0, 0, 0);


		// HP Bar Rendering
		hpBar[0]->SetPosition(position.x, position.y - 42.5f);
		hpBar[0]->Update(V, P);

		float val = 1.0f - hp / maxHp;

		hpBar[1]->SetScale(val, 1.0f);
		hpBar[1]->SetPosition(position.x, position.y - 42.5f);

		Vector2 pos1 = hpBar[0]->GetPosition();
		Vector2 pos2 = hpBar[1]->GetPosition();
		Vector2 size1 = hpBar[0]->GetTextureRealSize();
		Vector2 size2 = hpBar[1]->GetTextureRealSize();

		pos1.x += size1.x / 2;
		pos2.x -= size2.x / 2;

		float x = (pos2.x - pos1.x);

		if (hp < maxHp)
			hpBar[1]->SetPosition(position.x + (size2.x / val) + x, position.y - 42.5f);

		hpBar[1]->Update(V, P);
	}

	
	// Move
	{
		if (PRESS(VK_UP) || PRESS('W'))
		{
			movePos = Vector2(0.0f, 1.0f);
			SetRotation(0.0f, animation->GetRotation().y, 0.0f);
			moving = true;
		}
		else if (PRESS(VK_DOWN) || PRESS('S'))
		{
			movePos = Vector2(0.0f, -1.0f);
			SetRotation(0.0f, animation->GetRotation().y, 0.0f);
			moving = true;
		}

		if (PRESS(VK_LEFT) || PRESS('A'))
		{
			Vector2 pos = movePos;
			movePos = Vector2(-1.0f, pos.y);
			SetRotation(0.0f, 180.0f, 0.0f);
			moving = true;
		}
		else if (PRESS(VK_RIGHT) || PRESS('D'))
		{
			Vector2 pos = movePos;
			movePos = Vector2(1.0f, pos.y);
			SetRotation(0.0f, 0.0f, 0.0f);
			moving = true;
		}

		if (PRESS(VK_SHIFT))
		{
			movePos.x *= 3.0f;
			movePos.y *= 3.0f;
		}

		if (moving)
		{
			MoveObject();
		}
		else
		{
			animation->SetStop();
		}
	}

	animation->SetRotation(rotation);
	animation->SetPosition(position);
	animation->Update(V, P);

	// Attack
	{
		for (int i = 0; i < 8; i++)
		{
			if (!skills->GetSkillEquip(i))
				continue;
		
			SkillEffect* se = skills->GetSkillEffect(i);

			se->SetRotation(GetRotation());
			se->SetPosition(GetPosition());
			se->Update(VM, PM);
			se->UpdateEffect(VM, PM);
		}
	}

	// Collider
	{
		collider->SetPosition(GetPosition());
		collider->SetRotation(GetRotation());
		collider->SetScale(animation->GetTextureRealSize());

		collider->Update(V, P);
	}

	screenEffect->Update(V, P);
	fireEffect->Update(V, P);

	render = true;
}

void Player::Render()
{
	if (!IsActive())
		return;

	if (skills->GetSkillEquip(7))
	{
		SkillEffect* se8 = skills->GetSkillEffect(7);
		se8->Render();
	}

	animation->Render();

	for (int i = 0; i < 7; i++)
	{
		if (!skills->GetSkillEquip(i))
			continue;

		SkillEffect* se = skills->GetSkillEffect(i);

		se->Render();
		se->RenderEffect();
	}


	if (state == ATTACKED)
	{ 
		bloodEffect[1]->Render();
	}

	hpBar[0]->Render();
	hpBar[1]->Render();

	screenEffect->Render();
	fireEffect->Render();
	
	if (render)
	{
		if (visiblePlayerState)
		{
			wstring wstr = L"DEF : " + f_to_wstring(def);
			Vector2 pos = Vector2(GetPosition().x - 125.0f, hpBar[0]->GetPosition().y - 25.0f);
			CAMERA->VCToWC(pos);
			DirectWrite::RenderText(wstr, pos, 100, 100, 100, 20.0f);

			wstr = L"HP : " + f_to_wstring(hp) + L"/" + f_to_wstring(maxHp);
			pos = Vector2(GetPosition().x - 75.0f, hpBar[0]->GetPosition().y - 25.0f);
			CAMERA->VCToWC(pos);

			DirectWrite::RenderText(wstr, pos, 255, 0, 0, 20.0f);

			wstr = L"";

			if (oldmPos.x == -1.0f && oldmPos.y == -1.0f)
				wstr = L"↙";
			else if (oldmPos.x == 1.0f && oldmPos.y == -1.0f)
				wstr = L"↘";
			else if (oldmPos.x == -1.0f && oldmPos.y == 1.0f)
				wstr = L"↖";
			else if (oldmPos.x == 1.0f && oldmPos.y == 1.0f)
				wstr = L"↗";
			else if (oldmPos.x == 0.0f && oldmPos.y == -1.0f)
				wstr = L"↓";
			else if (oldmPos.x == 0.0f && oldmPos.y == 1.0f)
				wstr = L"↑";
			else if (oldmPos.x == 1.0f && oldmPos.y == 0.0f)
				wstr = L"→";
			else if (oldmPos.x == -1.0f && oldmPos.y == 0.0f)
				wstr = L"←";

			pos = Vector2(GetPosition().x + 50.0f, GetPosition().y + 50.0f);
			CAMERA->VCToWC(pos);

			DirectWrite::RenderText(wstr, pos, 255, 255, 0, 40.0f);
		}
	}

	render = false;

	{
		collider->Render();
	}
}

void Player::EquipBasic()
{
	// 기본 무기 : 채찍
	//ListCtrl* lc = (ListCtrl*)OBJMANAGER->FindObject("ListCtrl");
	//skills = (EquipList*)OBJMANAGER->FindObject("EquipList");
	//int no = 0;

	//if (!skills->GetSkillEquip(S_WHIP))
	//	skills->UpdateSkillLevel(S_WHIP, 1);
	//skills->UpdateSkillEquip(S_WHIP, true);
	//equipedSkillList[no] = S_WHIP;
	//
	//lc->UpdateItemCollect(S_WHIP * 2, true);
	//no++;

	///////////////////////////////////////////////////////////////

	//if (!skills->GetSkillEquip(S_MAGIC))
	//	skills->UpdateSkillLevel(S_MAGIC, 1);
	//skills->UpdateSkillEquip(S_MAGIC, true);
	//equipedSkillList[no] = S_MAGIC;
	//
	//lc->UpdateItemCollect(S_MAGIC * 2, true);
	//no++;

	///////////////////////////////////////////////////////////////

	//if (!skills->GetSkillEquip(S_SWORD))
	//	skills->UpdateSkillLevel(S_SWORD, 1);
	//skills->UpdateSkillEquip(S_SWORD, true);
	//equipedSkillList[no] = S_SWORD;
	//
	//lc->UpdateItemCollect(S_SWORD * 2, true);
	//no++;

	///////////////////////////////////////////////////////////////

	//if (!skills->GetSkillEquip(S_AXE))
	//	skills->UpdateSkillLevel(S_AXE, 1);
	//skills->UpdateSkillEquip(S_AXE, true);
	//equipedSkillList[no] = S_AXE;
	//
	//lc->UpdateItemCollect(S_AXE * 2, true);
	//no++;

	///////////////////////////////////////////////////////////////

	//if (!skills->GetSkillEquip(S_CROSS))
	//	skills->UpdateSkillLevel(S_CROSS, 1);
	//skills->UpdateSkillEquip(S_CROSS, true);
	//skills->GetSkillEffect(S_CROSS);
	//equipedSkillList[no] = S_CROSS;
	//
	//lc->UpdateItemCollect(S_CROSS * 2, true);
	//no++;

	///////////////////////////////////////////////////////////////

	//if (!skills->GetSkillEquip(S_BIBLE))
	//	skills->UpdateSkillLevel(S_BIBLE, 1);
	//skills->UpdateSkillEquip(S_BIBLE, true);
	//skills->GetSkillEffect(S_BIBLE);
	//equipedSkillList[no] = S_BIBLE;
	//
	//lc->UpdateItemCollect(S_BIBLE * 2, true);
	//no++;

	///////////////////////////////////////////////////////////////

	//if (!skills->GetSkillEquip(S_FIRE))
	//	skills->UpdateSkillLevel(S_FIRE, 1);
	//skills->UpdateSkillEquip(S_FIRE, true);
	//skills->GetSkillEffect(S_FIRE);
	//equipedSkillList[no] = S_FIRE;
	//
	//lc->UpdateItemCollect(S_FIRE * 2, true);
	//no++;

	///////////////////////////////////////////////////////////////

	//if (!skills->GetSkillEquip(S_GARLIC))
	//	skills->UpdateSkillLevel(S_GARLIC, 1);
	//skills->UpdateSkillEquip(S_GARLIC, true);
	//skills->GetSkillEffect(S_GARLIC);
	//equipedSkillList[no] = S_GARLIC;
	//
	//lc->UpdateItemCollect(S_GARLIC * 2, true);
	//no++;
}

void Player::MoveObject()
{
	Vector2 pos = GetPosition();

	if (movePos.x != 0.0f && movePos.y != 0.0f)
	{
		pos.x += (movePos.x * (3.0f * (1.0f + moveSpeed))) / sqrtf(2.0f);
		pos.y += (movePos.y * (3.0f * (1.0f + moveSpeed))) / sqrtf(2.0f);
	}
	else
	{
		pos.x += movePos.x * (3.0f * (1.0f + moveSpeed));
		pos.y += movePos.y * (3.0f * (1.0f + moveSpeed));
	}

	SetPosition(pos);
	
	animation->SetPlay(0);

	moving = false;
	oldmPos = movePos;
	movePos = Vector2(0.0f, 0.0f);
}

void Player::ItemAttack()
{	
	texture[0]->Render();
}

void Player::UseRosary()
{
	if (!screenEffect)
		cout << "생성 안됨" << endl;

	screenEffect->Reset();
	screenEffect->SetType(ScreenEffect::Type::GLOW_EFFECT);
}

void Player::UseNduja()
{
	fireEffect->Reset();
}

void Player::UseClock()
{
	screenEffect->Reset();
	screenEffect->SetType(ScreenEffect::Type::ICE_EFFECT);
}

void Player::UseChest()
{
	ChestMenu* chestMenu = (ChestMenu*)OBJMANAGER->FindObject("ChestMenu");
	chestMenu->Reset();
}

void Player::ApplyItem(int id)
{
	EquipList* list = (EquipList*)OBJMANAGER->FindObject("EquipList");

	switch (id)
	{
	case 0:
		//if (list->GetItemEquip(I_SPANISH - 8))
			power += list->GetItemValue(I_SPANISH - 8) / 100.0f;
		break;
	case 1:
		//if (list->GetItemEquip(I_ARMOR - 8))
			def += list->GetItemValue(I_ARMOR - 8);
		break;
	case 2:
		//if (list->GetItemEquip(I_BLACKHT - 8))
			maxHp += list->GetItemValue(I_BLACKHT - 8) / 100.0f * 120.0f;
		break;
	case 3:
		//if (list->GetItemEquip(I_REDHT - 8))
			recovery += list->GetItemValue(I_REDHT - 8);
		break;
	case 4:
		//if (list->GetItemEquip(I_BOOK - 8))
			coolDown += list->GetItemValue(I_BOOK - 8) / 100.0f;
		break;
	case 5:
		//if (list->GetItemEquip(I_CANDLE - 8))
			area += list->GetItemValue(I_CANDLE - 8) / 100.0f;
		break;
	case 6:
		//if (list->GetItemEquip(I_GLOVE - 8))
			skillSpeed += list->GetItemValue(I_GLOVE - 8) / 100.0f;
		break;
	case 7:
		//if (list->GetItemEquip(I_SPELL - 8))
			duration += list->GetItemValue(I_SPELL - 8) / 100.0f;
		break;
	case 8:
		//if (list->GetItemEquip(I_RING - 8))
			amount += list->GetItemValue(I_RING - 8);
		break;
	case 9:
		//if (list->GetItemEquip(I_WING - 8))
			moveSpeed += list->GetItemValue(I_WING - 8) / 100.0f;
		break;
	case 10:
		//if (list->GetItemEquip(I_CLOVER - 8))
			luck += list->GetItemValue(I_CLOVER - 8) / 100.0f;
		break;
	case 11:
		//if (list->GetItemEquip(I_CROWN - 8))
			growth += list->GetItemValue(I_CROWN - 8) / 100.0f;
		break;
	}
}

float Player::SetGUI(int id)
{
	switch (id)
	{
	case 0:
		return maxHp;
	case 1:
		return recovery;
	case 2:
		return def;
	case 3:
		return moveSpeed;
	case 4:
		return power;
	case 5:
		return area;
	case 6:
		return skillSpeed;
	case 7:
		return duration;
	case 8:
		return amount;
	case 9:
		return coolDown;
	case 10:
		return luck;
	case 11:
		return growth;
	default:
		return 0.0f;
	}
}


RECT Player::GetCollisionRect()
{
	RECT rect;

	Vector2 pPos = GetPosition();
	Vector2 size = animation->GetTextureRealSize();

	rect.left =		(LONG)(pPos.x - size.x / 2.5f);
	rect.right =	(LONG)(pPos.x + size.x / 2.5f);
	rect.top =		(LONG)(pPos.y + size.y / 2.5f);
	rect.bottom =	(LONG)(pPos.y - size.y / 2.5f);

	return rect;
}

bool Player::IsMonsterInRegion(Monster* mob)
{
	if (mob->GetState() == Monster::DEAD || mob->GetState() == Monster::FROZEN)
		return false;

	RECT r1 = this->GetCollisionRect();
	RECT r2 = mob->GetCollisionRect();

	if (r2.right > r1.left && r2.left < r1.right && r1.top > r2.bottom && r1.bottom < r2.top)
		return true;

	return false;
}

void Player::SetMobsInScreen()
{
	mobsInScreen.clear();
	mobsInScreen.shrink_to_fit();

	for (UINT i = 0; i < MAX_MOB; i++)
	{
		Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));

		Vector2 pos = mob->GetPosition();

		Vector2 pPos = CAMERA->GetPosition();
		float w = (float)MAIN->GetWidth() / 2;
		float h = (float)MAIN->GetHeight() / 2;

		if (pos.x >= pPos.x - w && pos.x <= pPos.x + w && pos.y <= pPos.y + h && pos.y >= pPos.y - h)
		{
			if (mob->IsActive())
				mobsInScreen.push_back(i);
		}
	}
}

void Player::AddExp(float val)
{
	if (LvLock)
		return;

	// Exp
	{
		float temp = 0.0f;

		exp += val + val * growth;

		do
		{
			if (exp >= 100.0f)
			{
				lvUpRemain++;

				if (level <= 50 && level % 10 == 0)
					power += 0.1f;

				cout << endl << "Level Up!" << endl;
				exp -= 100.0f;
			}

		} while (exp >= 100.0f);
	}
}

void Player::Reset()
{
	ResetAbility();

	for (int i = 0; i < 6; i++)
	{
		equipedSkillList[i] = -1;
		equipedItemList[i] = -1;
	}

	EquipBasic();
}

void Player::ResetAbility()
{
	power = 0.0f;
	def = 1.0f;
	maxHp = 120.0f;
	recovery = 0.0f;
	coolDown = 0.0f;
	area = 0.0f;
	skillSpeed = 0.0f;
	duration = 0.0f;
	amount = 0.0f;
	moveSpeed = 0.0f;
	magnet = 30;
	luck = 0.0f;
	growth = 0.0f;

	level = 1;
	lvUpRemain = 0;
	coin = 0;
	kill = 0;
	skillAmount = 0;
	itemAmount = 0;

	hp = maxHp;
	exp = 0.0f;
}

